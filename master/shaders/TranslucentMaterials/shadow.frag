#version 150
out vec4 fragColor;
void main()
{
        fragColor = vec4(gl_FragCoord.z);
        gl_FragDepth = gl_FragCoord.z;
}
