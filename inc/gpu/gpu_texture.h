#ifndef GPU_TEXTURE_H
#define GPU_TEXTURE_H
#include "texture.h"
#include "load_glad.h"




typedef struct t_gpu_texture
{
	// t_resource resource;
	t_texture *texture;
	GLuint id;
} t_gpu_texture;

t_gpu_texture *gpu_texture_add(const char *name, const char *asset_path, int channels);

#endif