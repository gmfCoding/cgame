#include "model.h"
#include "cont/vec_gpuvertex.h"
#include "load_glad.h"
#include "gpu/mesh.h"
#include <stddef.h>
#include "gldebug.h"
#include "clmath.h"

void gpu_mesh_line_from_normals(t_gpu_mesh *gm, t_gpu_line_list **line_list)
{
	t_mesh *const mesh = gm->mesh;
	size_t line_count = MIN(mesh->normals._len, mesh->vertices._len);

	t_gpu_line_list list = {0};

	list.count = line_count;
	list.lines = malloc(sizeof(t_gpu_normal) * list.count);

	for (size_t i = 0; i < line_count; i++)
	{
		t_vec3 normal = mesh->normals.data[i];
		t_vec3 vertex = mesh->vertices.data[i];
		list.lines[i].start = vertex;
		list.lines[i].end = (t_vec3){.x = vertex.x + normal.x * 0.1f, .y = vertex.y + normal.y * 0.1f, .z = vertex.z + normal.z * 0.1f	};
	}

	GLCall(glGenVertexArrays(1, &list.m_vao)); // Vertex  Array  Object
	GLCall(glGenBuffers(1, &list.m_vbo)); // Vertex  Buffer Object (temp)

	GLCall(glBindVertexArray(list.m_vao));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, list.m_vbo));


	**line_list = list;

	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(t_gpu_normal) * list.count, list.lines, GL_STATIC_DRAW)); 
	// Location of position
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(t_vec3), 0));
}

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
    
	vec_GPUVertex vertices = {0};
	printf("%d %d\n", gm->mesh->normals._len, gm->mesh->vertices._len);
	for (intptr_t i = 0; i < mesh->vertices._len; i++)
	{
		GPUVertex vertex;
		vertex.position = mesh->vertices.data[i];
		if(i < mesh->uvs._len)
			vertex.texcoord = mesh->uvs.data[i];
		if(i < mesh->normals._len)
			vertex.normal = mesh->normals.data[i];
		vec_GPUVertex_push_back(&vertices, vertex);
	}
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices._len * sizeof(GPUVertex), vertices.data, GL_STATIC_DRAW)); 
	// Location of position
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), 0));
	// Location of Texcoords
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), (GLvoid*)offsetof(GPUVertex, texcoord)));
	// Location of normals
	GLCall(glEnableVertexAttribArray(2));
 	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertex), (GLvoid*)offsetof(GPUVertex, normal)));

    // GLCall(glBindBuffer(GL_ARRAY_BUFFER, gm->m_vbo));
	// GLCall(glBufferData(GL_ARRAY_BUFFER, mesh->vertices._len * sizeof(float) * 3, mesh->vertices.data, GL_STATIC_DRAW)); 
	// GLCall(glEnableVertexAttribArray(0));
	// GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));

	// GLCall(glBindBuffer(GL_ARRAY_BUFFER, gm->m_vbo));
	// GLCall(glBufferData(GL_ARRAY_BUFFER, mesh->uvs._len * sizeof(float) * 2, mesh->uvs.data, GL_STATIC_DRAW)); 
	// GLCall(glEnableVertexAttribArray(1));
	// GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0));

	// GLCall(glBindBuffer(GL_ARRAY_BUFFER, gm->m_vbo));
	// GLCall(glBufferData(GL_ARRAY_BUFFER, mesh->normals._len * sizeof(float) * 3, mesh->normals.data, GL_STATIC_DRAW)); 
	// GLCall(glEnableVertexAttribArray(2));
	// GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0));

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
