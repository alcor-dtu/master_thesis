#version 430
uniform sampler2D vertices;
uniform sampler2D normals;
uniform sampler2D areas;
uniform int vertex_size;
uniform int vertex_tex_size;

// Shadows
uniform sampler2DShadow shadow;
uniform int shadowEnabled;
uniform mat4 Mat;


in vec3 vertex;
in vec3 normal;
in vec2 texcoord;

out vec4 _color;

uniform vec4 light_pos[50];
uniform vec4 light_diff[50];


uniform mat4 PVM;
uniform mat4 VM;
uniform mat4 M;
uniform mat3 N;

uniform float ior;
uniform vec3 absorption;
uniform vec3 scattering;
uniform vec3 mean_cosine;

uniform vec3 extinction;
uniform vec3 red_extinction;
uniform vec3 red_scattering;
uniform vec3 transmission;
uniform vec3 reduced_albedo;

uniform vec3 user_pos;

#define FRESNEL
const float M_PI = 3.141592654;


float sample_shadow_map(vec3 pos)
{
    vec4 light_pos = Mat * vec4(pos,1.0f);
    light_pos.z -= 0.0015; //bias to avoid shadow acne
    if(light_pos.x < 0.0 || light_pos.x > 1.0) return 1.0;
    if(light_pos.y < 0.0 || light_pos.y > 1.0) return 1.0;
    return texture(shadow,light_pos.xyz).r;
}


float C_1(float ni)
{
    float c;
    if(ni < 1.0f)
    {
        c = + 0.919317
            - 3.4793  * ni
            + 6.75335 * pow(ni,2)
            - 7.80989 * pow(ni,3)
            + 4.98554 * pow(ni,4)
            - 1.36881 * pow(ni,5);
    }
    else
    {
        c = - 9.23372
            + 22.2272  * ni
            - 20.9292  * pow(ni,2)
            + 10.2291  * pow(ni,3)
            - 2.54396  * pow(ni,4)
            + 0.254913 * pow(ni,5);
    }
    return c * 0.5f;
}

float C_Sigma(float ni)
{

    return 0.25 * (1 - 2 * C_1(ni));
}


float C_2(float ni)
{
    float c;
    if(ni < 1.0f)
    {
        c = + 0.828421
            - 2.62051  * ni
            + 3.36231  * pow(ni , 2)
            - 1.95284  * pow(ni , 3)
            + 0.236494 * pow(ni , 4)
            + 0.145787 * pow(ni,5);
    }
    else
    {
        c = - 1641.1
            + 135.926 / pow(ni,3)
            - 656.175 / pow(ni,2)
            + 1376.53 / ni
            + 1213.67 * ni
            - 568.556 * pow(ni,2)
            + 164.798 * pow(ni,3)
            - 27.0181 * pow(ni,4)
            + 1.91826 * pow(ni,5);
    }
    return c * (1.0f/3.0f);
}

float C_e(float ni)
{
    return 0.5 * (1 - 3 * C_2(ni));
}

vec3 refract2(vec3 inv, vec3 n, float n1, float n2)
{
    float eta = n1/n2;
    float c = dot(inv, n);
    return eta * (c * n - inv) - n * sqrt(max(0.0f,1 - eta * eta * (1 - c * c)));
}

vec2 fresnelAmplitudeTransmittance(vec3 inv, vec3 n, float n1, float n2)
{
    float cosin = dot(inv,n);
    vec3 refr = refract2(inv,n,n1,n2);
    float costr = dot(refr,-n);

    float t_s = (2 * n1 * cosin) / (n1 * cosin + n2 * costr);
    float t_p = (2 * n1 * cosin) / (n1 * costr + n2 * cosin);

    return vec2(t_s,t_p);
}


vec2 fresnelPowerTransmittance(vec3 inv, vec3 n, float n1, float n2)
{
    float cosin = dot(inv,n);
    vec3 refr = refract2(inv,n,n1,n2);
    float costr = dot(refr,-n);

    vec2 t = fresnelAmplitudeTransmittance(inv,n,n1,n2);

    return (n2 * costr) * ((t * t)/ (n1 * cosin));
}

float fresnel_T(vec3 inv, vec3 n, float n1, float n2)
{
    vec2 T = fresnelPowerTransmittance(inv,n,n1,n2);
    return 0.5 * (T.x + T.y);
}


vec3 bssrdf(const vec3 xi, const vec3 wi, const vec3 ni, const vec3 xo, const vec3 wo, const vec3 no)
{
    float l = length(xo - xi);

    float ntr = ior;
    float nin = 1.0f; //air
    float eta = ntr / nin;


    vec3 D = (2 * absorption + red_scattering) / (3 * red_extinction * red_extinction);
    float A = (1 + 3 * C_2(eta)) / (1 - 2 * C_1(eta));

    vec3 zr = vec3(1.0f)/vec3(red_extinction);
    vec3 zv = - zr - 4.0f * A * D;

    vec3 dr;
    vec3 dv;

    vec3 r = vec3(l);
    dr = sqrt(r * r + zr * zr);
    dv = sqrt(r * r + zv * zv);

    vec3 tr = sqrt(absorption / D);

    vec3 C1 = (zr / (dr * dr)) * (tr * dr + vec3(1.0f));
    vec3 C2 = (zv / (dv * dv)) * (tr * dv + vec3(1.0f));

    float CE = C_e(eta);
    vec3 reducedCSigma = vec3(C_Sigma(eta)) / D;
    vec3 coeff = (reduced_albedo * reduced_albedo) / (4.0f * M_PI);
    vec3 real = (CE * C1 + reducedCSigma) * (exp(- tr * dr)/dr);
    vec3 virt = (CE * C2 + reducedCSigma) * (exp(- tr * dv)/dv);

    vec3 S = coeff * (real - virt);
    float Ti = fresnel_T(wi,ni,nin,ntr);
    float To = fresnel_T(wo,no,nin,ntr);

    S *= (1.0f/M_PI)* Ti; //* To;

    S = max(vec3(0.0f),S);


    return S;
}


void main()
{
    vec3 no = normalize(vec3(M * vec4(normal,0.0f)));
    vec4 p = (M * vec4(vertex,1.0f));

    vec3 wo = normalize(user_pos - p.xyz);
    vec3 xo = p.xyz;

    vec3 Lo = vec3(0.0f);

    float s = 1.0f / (vertex_tex_size-1);
    vec3 Li_base = vec3(light_diff[0]);
    vec3 wi = normalize(light_pos[0].xyz);

    for(int i = 0; i < vertex_tex_size; i++)
    {

        for(int j = 0; j < vertex_tex_size; j++)
        {
            //if(j * vertex_tex_size + i >= vertex_size)
            //{
            //    continue;
            //}

            vec2 coord = vec2(j * s, i * s);
            vec3 ni = texelFetch(normals,ivec2(i,j),0).xyz;
            ni = normalize(vec3(M * vec4(ni,0.0f)));
            vec3 xi = texelFetch(vertices,ivec2(i,j),0).xyz;
            xi = vec3(M * vec4(xi,1.0f));
            float area =  texelFetch(areas,ivec2(i,j),0).x;

            float dot_n_w = dot(ni,wi);

            float visibility = dot_n_w;

            if(shadowEnabled > 0)
            {
                visibility = sample_shadow_map(xi);
            }

            if(visibility > 0.0f)
            {
                vec3 BSSRDF = bssrdf(xi,wi,ni,xo,wo,no);
                Lo += Li_base * dot_n_w * BSSRDF * area;
            }
        }
    }

    _color = vec4(Lo,1.0f);

    gl_Position = PVM * vec4(vertex,1);
}
