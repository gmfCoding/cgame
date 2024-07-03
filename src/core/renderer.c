#include "load_glad.h"
#include "gldebug.h"
#include "entity.h"
#include "material.h"
#include "camera.h"
#include <cglm/cam.h>
#include <cglm/mat4.h>

void render_entities(t_render_ctx *ctx)
{
    c_foreach(i, set_entptr, ctx->entities)
        render_entity((*i.ref));
}

void render_gpu_mesh(t_gpu_mesh *mesh)
{
    GLCall(glBindVertexArray(mesh->m_vao));
    GLCall(glDrawElements(GL_TRIANGLES, mesh->m_size, GL_UNSIGNED_INT, NULL));
}

t_mesh_renderer *mesh_renderer_create(t_gpu_mesh *mesh, t_material *material)
{
    t_mesh_renderer *renderer = malloc(sizeof(t_mesh_renderer));
    *renderer = (t_mesh_renderer){0};
    renderer->material = material;
    renderer->mesh = mesh;
    return renderer;
}

void render_mesh_renderer(t_render_ctx *context, t_transform *transform, t_mesh_renderer *renderer)
{
    if (transform == NULL || renderer->mesh == NULL)
        return;
    t_mat_prop *prop;

    // apply materials
    if ((prop = material_prop_get(renderer->material, "MVP")) != NULL)
    {
        transform_get_mat4(transform, prop->value.mat);
        glm_mat4_mul(context->camera.premultPV, prop->value.mat, prop->value.mat);
        material_prop_update(renderer->material, prop);
    }
    material_apply(renderer->material);
    render_gpu_mesh(renderer->mesh);
}

void render_entity(t_entity *entity)
{
    if (entity->renderer == NULL || entity->render_ctx == NULL)
        return;
    render_mesh_renderer(entity->render_ctx, &entity->transform, entity->renderer);
}

void entity_render_attach(t_render_ctx *render, t_entity *entity)
{
    entity->render_ctx = render;
    set_entptr_push(&render->entities, entity);
}

void entity_render_deattach(t_render_ctx *render, t_entity *entity)
{
    entity->render_ctx = NULL;
    set_entptr_erase(&render->entities, entity);
}