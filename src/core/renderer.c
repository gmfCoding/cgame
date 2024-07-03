#include "load_glad.h"
#include "gldebug.h"
#include "entity.h"

void render_entities(t_render_ctx *ctx)
{
    c_foreach (i, set_entptr, ctx->entities)
        render_entity((*i.ref));
}

void render_gpu_mesh(t_gpu_mesh *mesh)
{
    GLCall(glBindVertexArray(mesh->m_vao));
    GLCall(glDrawElements(GL_TRIANGLES, mesh->m_size, GL_UNSIGNED_INT, NULL));
}

void render_mesh_renderer(const t_transform *transform, t_mesh_renderer *renderer)
{
    if (transform == NULL || renderer->mesh == NULL)
        return;
    // apply materials
    render_gpu_mesh(renderer->mesh);
}

void render_entity(t_entity *entity)
{
    if (entity->renderer == NULL || entity->render_ctx == NULL)
        return;
    render_mesh_renderer(&entity->transform, entity->renderer);
}

/// @brief Attaches an entity to the render context
/// This allows the entity to be rendered
/// @param render 
/// @param entity 
void entity_render_attach(t_render_ctx *render, t_entity *entity)
{
    entity->render_ctx = render;
    set_entptr_push(&render->entities, entity);
}

void entity_render_attach(t_render_ctx *render, t_entity *entity)
{
    entity->render_ctx = NULL;
    set_entptr_erase(&render->entities, entity);
}