#version 330 core

out vec4 FragColour;


uniform vec3 colour;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D diffuse_tex;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColour;

	float specularStrength = 5.0;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 255);
	vec3 specular = specularStrength * spec * lightColour;  


	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColour;
	vec3 result = (ambient + diffuse + specular) * vec3(texture(diffuse_tex, TexCoord));
	//FragColour = vec4(vec3(), 1.0);
	FragColour = texture(diffuse_tex, TexCoord);
	//FragColour = vec4(TexCoord, 0.0, 1.0);
}