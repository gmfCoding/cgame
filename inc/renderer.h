#ifndef RENDERER_H 
#define RENDERER_H

#include "cont/set_entptr.h"


typedef struct s_mesh_renderer t_mesh_renderer;
typedef struct s_transform t_transform;
typedef struct s_entity t_entity;
typedef struct s_material t_material;

typedef struct s_render_ctx
{
    set_entptr entities;

} t_render_ctx;

t_mesh_renderer* mesh_renderer_create(t_gpu_mesh *mesh, t_material *material);

void render_entities(t_render_ctx *ctx);

void render_gpu_mesh(t_gpu_mesh *mesh);

void render_mesh_renderer(const t_transform *transform, t_mesh_renderer *renderer);

void render_entity(t_entity *entity);

/// @brief Attaches an entity to the render context
/// This allows the entity to be rendered
/// @param render 
/// @param entity 
void entity_render_attach(t_render_ctx *render, t_entity *entity);

void entity_render_deattach(t_render_ctx *render, t_entity *entity);

#endif