#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;  

out vec4 FragColour;

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
	// ambient
	vec3 ambient = light.ambient * material.ambient;

	// diffuse
	vec3 norm = normalize(vs.Normal);
	vec3 lightDir = normalize(light.position - vs.FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse  * (diff * material.diffuse);

	// specular
	vec3 viewDir = normalize(viewPos - vs.FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result  = ambient + diffuse + specular;
	FragColour = vec4(result, 1.0);
}