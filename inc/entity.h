#ifndef ENTITY_H
#define ENTITY_H

#include "vector3.h"
#include "gpu/mesh.h"
#include "renderer.h"

typedef enum
{
    ET_BASE,

} t_entitytype;

typedef struct 
{
    t_vec3 position;
    t_vec3 rotation;
    t_vec3 scale;

    /* data */
} t_transform;

typedef struct
{
    t_gpu_mesh *mesh;
} t_mesh_renderer;

typedef struct
{
    t_transform transform;
    t_mesh_renderer *renderer;

    t_render_ctx *render_ctx;
} t_entity;
#endif