#version 430

uniform sampler2D tex;  // Uniform specifying the texture unit

in vec4 _color;
out vec4 fragColor;

uniform float gamma;
void main()
{
    fragColor = pow(_color, vec4(1.0f/gamma));

}



