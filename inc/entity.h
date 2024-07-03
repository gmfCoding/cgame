#ifndef ENTITY_H
#define ENTITY_H

#include "vector3.h"
#include "gpu/mesh.h"
#include "renderer.h"

typedef struct s_material t_material;

typedef enum t_entitytype
{
    ET_BASE,

} t_entitytype;

typedef struct s_transform
{
    t_vec3 position;
    t_vec3 rotation;
    t_vec3 scale;

    /* data */
} t_transform;

typedef struct s_mesh_renderer
{
    t_gpu_mesh *mesh;
	t_material *material;
} t_mesh_renderer;

typedef struct s_entity
{
    t_transform transform;
    t_mesh_renderer *renderer;

    t_render_ctx *render_ctx;
} t_entity;


t_entity *entity_create(t_entitytype type);
void entity_destroy(t_entity *entity);

#endif