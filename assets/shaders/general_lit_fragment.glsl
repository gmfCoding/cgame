#version 330 core

struct Material {
    sampler2D diffuse;
	sampler2D specular;
    float shininess;

	bool disable_lighting;
};

struct PointLight {

	bool enabled;
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// IMPORTANT THAT NR_POINT_LIGHTS is defined in the application and should match the number of point lights used in the scene.
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionalLight directional;
uniform Material material;

uniform float time;
uniform vec3 viewPos;
uniform sampler2D diffuse_tex;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs;

out vec4 FragColour;

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, vs.TexCoord));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, vs.TexCoord));  
	vec3 specular = light.specular * spec * vec3(texture(material.specular, vs.TexCoord));
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
					light.quadratic * (distance * distance)); 

	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, vs.TexCoord));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, vs.TexCoord));  
	vec3 specular = light.specular * spec * vec3(texture(material.specular, vs.TexCoord));
	return (ambient + diffuse + specular);
}

void main()
{
	if (material.disable_lighting)
	{
		FragColour = vec4(vec3(texture(material.diffuse, vs.TexCoord)), 1.0);
		return;
	}
	vec3 norm = normalize(vs.Normal);
	vec3 viewDir = normalize(viewPos - vs.FragPos);
	vec3 result = CalculateDirectionalLight(directional, norm, vs.FragPos, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalculatePointLight(pointLights[i], norm, vs.FragPos, viewDir);
	FragColour = vec4(result, 1.0);  
}