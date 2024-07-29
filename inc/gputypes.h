#ifndef GPUTYPES_H
# define GPUTYPES_H

#include <load_glad.h>

#include <inttypes.h>
#include "vector2.h"
#include "vector3.h"

typedef struct {
    t_vec3 position;
    t_vec2 texcoord;
	t_vec3 normal;
} GPUVertex;

typedef uint32_t GPUIndex;

typedef struct s_texture t_texture;

typedef struct t_gpu_texture
{
	// t_resource resource;
	t_texture *texture;
	GLuint id;
} t_gpu_texture;

#endif