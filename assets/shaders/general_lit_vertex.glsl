// Vertex Shader
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


uniform vec2 texcoord_scale;
uniform vec2 texcoord_offset;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = aNormal;
    vs_out.TexCoord = (aTexCoord * texcoord_scale) + texcoord_offset;
}
