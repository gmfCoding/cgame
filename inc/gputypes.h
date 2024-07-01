#ifndef GPUTYPES_H
# define GPUTYPES_H

#include <inttypes.h>
#include "vector2.h"
#include "vector3.h"

typedef struct {
    t_vec3 position;
    t_vec2 texcoord;
	t_vec3 normal;
} GPUVertex;

typedef uint32_t GPUIndex;
#endif