#include "load_glad.h"
#include "gldebug.h"
#include "entity.h"
#include "material.h"
#include "camera.h"
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include "logging.h"

void render_entities(t_render_ctx *ctx)
{
    c_foreach(i, set_entptr, ctx->entities)
        render_entity((*i.ref));
}

void render_gpu_line_list(t_gpu_line_list *line_list)
{
	GLCall(glBindVertexArray(line_list->m_vao));
	GLCall(glDrawArrays(GL_LINES, 0, line_list->count * 2));
}

void render_gpu_mesh_mode_indices(t_gpu_mesh *mesh)
{
    GLCall(glBindVertexArray(mesh->m_vao));
    GLCall(glDrawElements(GL_TRIANGLES, mesh->m_size, GL_UNSIGNED_INT, NULL));
}

void render_gpu_mesh_mode_points_only(t_gpu_mesh *mesh)
{
    GLCall(glBindVertexArray(mesh->m_vao));
	GLCall(glPointSize(20.0f));
    GLCall(glDrawArrays(GL_POINTS, 0, mesh->m_size));
}


t_mesh_renderer *mesh_renderer_create(t_gpu_mesh *mesh, t_material *material)
{
    t_mesh_renderer *renderer = malloc(sizeof(t_mesh_renderer));
    *renderer = (t_mesh_renderer){0};
    renderer->material = material;
	renderer->render_mode = MRMT_INDICES;
    renderer->mesh = mesh;
    return renderer;
}

static void render_mesh_render_default_props(t_render_ctx *context, t_material* material, t_transform *transform)
{
	{
		t_mat_prop *prop = material_prop_get(material, "MVP");
		if (prop != NULL)
		{
			glm_mat4_identity(prop->value.mat);
			transform_get_mat4(transform, prop->value.mat);
			glm_mat4_mul(context->camera.premultPV, prop->value.mat, prop->value.mat);
			material_prop_update(material, prop);
		}
	}

	{
		t_mat_prop *prop = material_prop_get(material, "model");
		if (prop != NULL)
		{
			glm_mat4_identity(prop->value.mat);
			transform_get_mat4(transform, prop->value.mat);
			material_prop_update(material, prop);
		}
	}

	{
		t_mat_prop *prop = material_prop_get(material, "viewPos");
		if (prop != NULL)
		{
			glm_vec3_copy(context->camera.transform.position, prop->value.f3);
			material_prop_update(material, prop);
		}
	}

	{
		t_mat_prop *prop = material_prop_get(material, "view");
		if (prop != NULL)
		{
			glm_mat4_identity(prop->value.mat);
			glm_mat4_copy(context->camera.view, prop->value.mat);
			material_prop_update(material, prop);
		}
	}

	{
		t_mat_prop *prop = material_prop_get(material, "proj");
		if (prop != NULL)
		{
			glm_mat4_identity(prop->value.mat);
			glm_mat4_copy(context->camera.projection, prop->value.mat);
			material_prop_update(material, prop);
		}
	}

	{
		t_mat_prop *prop = material_prop_get(material, "proj");
		if (prop != NULL)
		{
			glm_mat4_identity(prop->value.mat);
			glm_mat4_copy(context->camera.projection, prop->value.mat);
			material_prop_update(material, prop);
		}
	}

	if (context->active_shader != material->shader)
	{
		t_mat_prop *prop = material_prop_get(material, "lightPos");
		if (prop != NULL)
		{
			prop->value = (t_mat_prop_value){.f3 = {context->lightPos[0], context->lightPos[1], context->lightPos[2]}};
			material_prop_update(material, prop);
		}

		t_mat_prop *lightColour = material_prop_get(material, "lightColour");
		if (lightColour != NULL)
		{
			lightColour->value = (t_mat_prop_value){.f3 = {context->lightColour[0], context->lightColour[1], context->lightColour[2]}};
			material_prop_update(material, lightColour);
		}
	}
} 

void render_mesh_renderer(t_render_ctx *context, t_transform *transform, t_mesh_renderer *renderer)
{
	if (transform == NULL || renderer->mesh == NULL)
		return;

	render_mesh_render_default_props(context, renderer->material, transform);

	material_apply(context, renderer->material);
	if (renderer->render_mode == MRMT_INDICES)
		render_gpu_mesh_mode_indices(renderer->mesh);
	else if (renderer->render_mode == MRMT_POINTS_ONLY)
		render_gpu_mesh_mode_points_only(renderer->mesh);
	if (renderer->render_mesh_normals)
	{
		t_gpu_line_list *line_list = &renderer->mesh->line_list;
		if (line_list->count == 0)
			gpu_mesh_line_from_normals(renderer->mesh, &line_list);
		t_material *material = material_system_mat_get(&context->material_system, "normal_line_material");
		if (material == NULL)
			return;
		render_mesh_render_default_props(context, material, transform);
		te_logf(LOG_LEVEL_INFO, "renderer", "Rendering mesh normals");
		material_apply(context, material);
		render_gpu_line_list(&renderer->mesh->line_list);
	}
}

void render_entity(t_entity *entity)
{
    if (entity->renderer == NULL || entity->render_ctx == NULL)
		return;
	char debug_str[64];
	if (entity->debug_name[0] != '\0')
		snprintf(debug_str, sizeof(debug_str), "Rendering entity %d ('%s')", entity->entity_id, entity->debug_name);
	else
		snprintf(debug_str, sizeof(debug_str), "Rendering entity %d", entity->entity_id);
	glPushDebugGroupKHR(GL_DEBUG_SOURCE_APPLICATION, 0, -1, debug_str
	);
	te_logf(LOG_LEVEL_INFO, "renderer", "%s {", debug_str);
    render_mesh_renderer(entity->render_ctx, &entity->transform, entity->renderer);
	te_logf(LOG_LEVEL_INFO, "renderer", "} Finished rendering entity %d", entity->entity_id);
	glPopDebugGroupKHR();
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