#ifndef GPU_MESH_H
#define GPU_MESH_H
#include "model.h"

typedef struct
{
	bool dirty;
	bool dynamic;

    t_mesh *mesh;

	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_vao;
	int m_size;

	bool uvs;
	bool normals;
} t_gpu_mesh;


t_gpu_mesh *gpu_mesh_add(t_mesh *mesh);
void gpu_mesh_vertices_refresh(t_gpu_mesh *gm);
void gpu_mesh_indices_refresh(t_gpu_mesh *gm);
#endif
