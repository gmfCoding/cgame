#ifndef ENTITY_H
#define ENTITY_H

#include "vector3.h"
#include "gpu/mesh.h"
#include "renderer.h"
#include "transform.h"

typedef struct s_material t_material;
typedef struct s_render_ctx t_render_ctx;



typedef enum t_entitytype
{
    ET_BASE,
    ET_LIGHT,

} t_entitytype;


typedef enum t_mesh_render_mode
{
    MRMT_INDICES,
    MRMT_POINTS_ONLY,
} t_mesh_render_mode_type;

typedef struct s_mesh_renderer
{
    t_gpu_mesh *mesh;
	t_material *material;

    t_mesh_render_mode_type render_mode;
    bool render_mesh_normals;
} t_mesh_renderer;

typedef struct s_entity
{
    uint32_t entity_id;
    char debug_name[32];
    t_transform transform;
    t_mesh_renderer *renderer;

    t_render_ctx *render_ctx;
} t_entity;

t_entity* entity_create_unowned(t_entitytype type, t_entity* entity);
t_entity* entity_create(t_entitytype type);
void entity_destroy(t_entity *entity);

#endif