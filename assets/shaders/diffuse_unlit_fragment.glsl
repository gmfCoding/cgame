#version 330 core

out vec4 FragColour;
uniform vec3 colour;
uniform sampler2D diffuse;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs;

void main()
{
	FragColour = texture(diffuse, vs.TexCoord);
}