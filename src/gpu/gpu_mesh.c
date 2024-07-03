#include "model.h"
#include "cont/vec_gpuvertex.h"
#include "load_glad.h"
#include "gpu/mesh.h"
#include <stddef.h>
#include "gldebug.h"

t_gpu_mesh *gpu_mesh_add(t_mesh *mesh)
{
    t_gpu_mesh *gmesh = malloc(sizeof(t_gpu_mesh));
    *gmesh = (t_gpu_mesh){0};

    gmesh->normals = mesh->normals._len >= 1 && mesh->normals._len == mesh->vertices._len;
	gmesh->uvs = mesh->uvs._len >= 1 && mesh->uvs._len == mesh->vertices._len;
	gmesh->mesh = mesh;
	GLCall(glGenVertexArrays(1, &gmesh->m_vao)); // Vertex  Array  Object

	GLCall(glGenBuffers(1, &gmesh->m_vbo)); // Vertex  Buffer Object (temp)
	GLCall(glGenBuffers(1, &gmesh->m_ibo)); // Element Buffer Object (temp)

    GLCall(glBindVertexArray(gmesh->m_vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, gmesh->m_vbo));

	gpu_mesh_vertices_refresh(gmesh);
	gpu_mesh_indices_refresh(gmesh);

    GLCall(glBindVertexArray(0));
	return gmesh;
}

void gpu_mesh_vertices_refresh(t_gpu_mesh *gm)
{ 
	t_mesh *const mesh = gm->mesh;
    GLCall(glBindVertexArray(gm->m_vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, gm->m_vbo));
    
	vec_GPUVertex vertices_uv = {0};
	for (intptr_t i = 0; i < mesh->vertices._len; i++)
	{
		GPUVertex vertex;
		vertex.position = mesh->vertices.data[i];
		if(gm->uvs)
			vertex.texcoord = mesh->uvs.data[i];
		if(gm->normals)
			vertex.normal = mesh->normals.data[i];
		vec_GPUVertex_push_back(&vertices_uv, vertex);
	}
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices_uv._len * sizeof(GPUVertex), vertices_uv.data, GL_STATIC_DRAW)); 
	// Location of position
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), 0));
	// Location of Texcoords
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), (GLvoid*)offsetof(GPUVertex, texcoord)));
	// Location of normals
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), (GLvoid*)offsetof(GPUVertex, normal)));
}

void gpu_mesh_indices_refresh(t_gpu_mesh *gm)
{
    GLCall(glBindVertexArray(gm->m_vao));

	gm->m_size = gm->mesh->indices._len;

    // NOTE: Do we need a currently bound vao to buffer/bind the ibo?
    // Fill the currently bound GL_ELEMENT_ARRAY_BUFFER buffer (ibo) with the data in indices
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gm->m_ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, gm->m_size * sizeof(GPUIndex), gm->mesh->indices.data, gm->dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}
