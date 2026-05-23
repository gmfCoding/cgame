#include "engine.h"
#include "io.h"
#include "model.h"
#include "asset.h"
#include "entity.h"
#include "renderer.h"
#include "entity/light.h"
#include "core/procmesh.h"

struct scene_proc_meshs
{
    t_model* cube_model;
	t_material* shared;
	t_pm_grid pm_grid;
};

#include "material_system.h"
#include "lighting.h"
#include "io.h"
#include "gpu/gpu_texture.h"
#include "util.h"
#include "random.h"
#include "stb_image.h"

static void materials_setup(t_material_system *system)
{
	char path[PATH_MAX];
	// {
	// 	path[0] = '\0';
	// 	asset_get_path(path, 2, "shaders", "test_vertex.glsl");
	// 	char *basic_vertex = strdup(path);
		
	// 	path[0] = '\0';
	// 	asset_get_path(path, 2, "shaders", "test_fragment.glsl");
	// 	char *basic_fragment = strdup(path);
		
	// 	GLuint program;
	// 	gpu_shader_program_compile_vert_frag(basic_vertex, basic_fragment, &program);
	// 	t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "test_shader"});	
	// 	t_material *basic = material_system_mat_create(system, (t_material){.shader = shader, .name = "test_material"});
	// 	GLuint tex = loadTexture(asset_get_path(path, 2, "textures", "uv.png"));
    //     material_prop_add_new(basic, "MVP", MPT_MAT4, MPT_DEFAULT);
    //     material_prop_add_new(basic, "texture1", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = tex, .slot = 0}});

	// 	free(basic_fragment);
	// 	free(basic_vertex);
	// }
	{
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "general_lit_vertex.glsl");
		char *general_vertex = strdup(path);
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "general_lit_fragment.glsl");
		char *general_fragment = strdup(path);

		GLuint program;
		gpu_shader_program_compile_vert_frag(general_vertex, general_fragment, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "general_lit_shader"});	
		t_material *lit = material_system_mat_create(system, (t_material){.shader = shader, .name = "general_lit_shader"});

		t_gpu_texture texture;
		gpu_texture_add(&texture, "crate", asset_get_path(path, 2, "textures", "crate.png"), 4);

		t_gpu_texture texture_specular;
		gpu_texture_add(&texture_specular, "crate_spec", asset_get_path(path, 2, "textures", "crate_spec.png"), 4);

		material_prop_add_new(lit, "material.diffuse", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture.id, .slot = 0}});
		material_prop_add_new(lit, "material.specular", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture_specular.id, .slot = 1}});
		material_prop_add_new(lit, "material.shininess", MPT_FLOAT1, (t_mat_prop_value){.f1={64.0f}});
		material_prop_add_new(lit, "material.disable_lighting", MPT_BOOL, (t_mat_prop_value){.b={false}});

		material_prop_add_new(lit, "texcoord_scale", MPT_FLOAT2, (t_mat_prop_value){.f2={1.0f, 2.0f}});
		material_prop_add_new(lit, "texcoord_offset", MPT_FLOAT2, (t_mat_prop_value){.f2={0.0f, 0.0f}});

		material_prop_add_new_light(LT_DIRECTION, 0, lit);
		lit->max_lights = 4;
		for (int i = 0; i < lit->max_lights; i++)
		{
			material_prop_add_new_light(LT_POINT, i, lit);
		}

		material_prop_add_new(lit, "model", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "view", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "proj", MPT_MAT4, MPT_DEFAULT);

		material_prop_add_new(lit, "viewPos", MPT_FLOAT3, MPT_DEFAULT);

		free(general_fragment);
		free(general_vertex);
	}

	{
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "normal_line_vertex.glsl");
		char *vert = strdup(path);

		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "normal_line_fragment.glsl");
		char *frag = strdup(path);

		GLuint program;
		gpu_shader_program_compile_vert_frag(vert, frag, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "normal_line_shader"});
		t_material *mat = material_system_mat_create(system, (t_material){.shader = shader, .name = "normal_line_material"});

		material_prop_add_new(mat, "MVP", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(mat, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3 = {0, 1, 1}});

		free(vert);
		free(frag);
	}
}

static void setup_camera(t_engine* engine)
{
    engine->render_context.camera = CAMERA_DEFAULT;
	glm_vec3_copy((vec3){0,0,10}, engine->render_context.camera.transform.position);
	camera_init(&engine->render_context.camera, 70.0f, (float)engine->width / (float)engine->height, 0.1f, 100.0f);
}

static void setup_meshes(t_engine* engine, struct scene_proc_meshs* scene)
{
	pm_grid_create(&scene->pm_grid, 10, 20, true);

	t_entity *ent = entity_create(ET_BASE);
	ent->transform.scale[0] = 1;
	ent->transform.scale[1] = 1;
	ent->transform.scale[2] = 1;
	t_material *material = material_system_mat_get(&engine->render_context.material_system, "general_lit_shader");
	scene->shared = material; 
	ent->renderer = mesh_renderer_create(scene->pm_grid.gpu_mesh, material);
	ent->renderer->render_mesh_normals = true;
	entity_render_attach(&engine->render_context, ent);
}


static e_engine_hook_result ca_render_thread(t_engine* engine, void* scene_ptr)
{
	struct scene_proc_meshs* const scene = (struct scene_proc_meshs*)scene_ptr;

    const double delta_time = engine->delta_time;
    static float angle = 0.0;
    angle += delta_time;

    return ENGINE_HOOK_RESULT_CONTINUE;
}

#include "logging.h"
#include "vector4.h"

static e_engine_hook_result scene_start(t_engine* engine, void* scene_ptr)
{
	struct scene_proc_meshs* const scene = (struct scene_proc_meshs*)scene_ptr;
	te_logf(LOG_LEVEL_INFO, "scene", "Starting scene proc_meshs");
    setup_meshes(engine, scene);
	engine->clear_colour = (t_vec4){.v = {0.05, 0.05, 0.05, 0.05}};
    return ENGINE_HOOK_RESULT_CONTINUE;
}

static e_engine_hook_result scene_awake(t_engine* engine, void* scene_ptr)
{
	UNUSED(scene_ptr);
    setup_camera(engine);
    materials_setup(&engine->render_context.material_system);

    return ENGINE_HOOK_RESULT_CONTINUE;
}

e_engine_hook_result scene_setup_proc_meshs(t_engine* engine, void* context)
{
	UNUSED(context);
    struct scene_proc_meshs* scene = malloc(sizeof(*scene));
    *scene = (struct scene_proc_meshs){0};

    engine->posthook = (void*)&scene_awake;
    engine->hook_context = scene;

    engine->scene = (void*)&scene_start;
    engine->scene_context = scene;

    engine->render_thread_frame_interhook = (void*)&ca_render_thread;
    engine->render_thread_context = scene;
    return ENGINE_HOOK_RESULT_CONTINUE;
}
