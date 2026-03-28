// Geometry Shader
#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 9) out;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} gs_in[];

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float arrowLength = 0.5;
uniform float explodeFactor = 0.2;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main() {

    vec3 faceNormal = normalize(gs_in[0].Normal + gs_in[1].Normal + gs_in[2].Normal);
	// Emit original triangle
    for(int i = 0; i < 3; i++) {
		vec3 vertexPos = gs_in[i].FragPos;
		vec3 explodedPos = vertexPos + faceNormal * explodeFactor;
        FragPos = explodedPos;


        Normal = gs_in[i].Normal;
        TexCoord = gs_in[i].TexCoord;
        gl_Position = proj * view * vec4(explodedPos, 1.0);
        EmitVertex();
    }

    // For each vertex in the triangle
    for(int i = 0; i < 3; i++) {
        vec3 vertexPos = gs_in[i].FragPos;
		vec3 explodedPos = vertexPos + faceNormal * explodeFactor;

        vec3 normal = normalize(gs_in[i].Normal);
        
        // Start point (vertex position)
        FragPos = explodedPos;
        Normal = normal;
        TexCoord = gs_in[i].TexCoord;
        gl_Position = proj * view * vec4(explodedPos, 1.0);
        EmitVertex();
        
        // End point (vertex + normal * length)
        vec3 arrowTip = explodedPos + normal * arrowLength;
        FragPos = arrowTip;
        Normal = normal;
        TexCoord = gs_in[i].TexCoord;
        gl_Position = proj * view * vec4(arrowTip, 1.0);
        EmitVertex();
        
        EndPrimitive();
    }
}
