#version 330 core

out vec4 FragColour;


uniform vec3 colour;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D diffuse_tex;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs;

uniform float time;
out vec4 fragment;

void main()
{
	vec3 norm = normalize(vs.Normal);
	vec3 lightDir = normalize(lightPos - vs.FragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColour;

	float specularStrength = 1.0;
	vec3 viewDir = normalize(viewPos - vs.FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 255);
	vec3 specular = specularStrength * spec * lightColour;  


	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * lightColour;
	vec3 result = (ambient + diffuse + specular) * vec3(texture(diffuse_tex, vs.TexCoord));
	FragColour = vec4(result, 1.0);
}