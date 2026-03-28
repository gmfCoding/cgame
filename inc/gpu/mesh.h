#ifndef GPU_MESH_H
#define GPU_MESH_H
#include "model.h"
#include "load_glad.h"

typedef struct s_gpu_line
{
	t_vec3 start;
	t_vec3 end;
} t_gpu_normal;

typedef struct s_gpu_line_list
{
	t_gpu_normal *lines;
	size_t count;
	GLuint m_vbo;
	GLuint m_vao;

} t_gpu_line_list;

typedef struct s_gpu_mesh
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
	t_gpu_line_list line_list;
} t_gpu_mesh;


t_gpu_mesh *gpu_mesh_add(t_mesh *mesh);
void gpu_mesh_vertices_refresh(t_gpu_mesh *gm);
void gpu_mesh_indices_refresh(t_gpu_mesh *gm);
void gpu_mesh_line_from_normals(t_gpu_mesh *gm, t_gpu_line_list **line_list);

#endif
