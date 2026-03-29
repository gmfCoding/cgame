#include "engine.h"
#include "io.h"
#include "model.h"
#include "asset.h"
#include "entity.h"
#include "renderer.h"

struct scene_cube_array
{
    t_entity** cubes;
    int count;
    t_model* cube_model;
    t_gpu_mesh* cube_gpu_mesh;
    t_entity *light;
	t_material* shared;
};

#include "material_system.h"
#include "io.h"
#include "gpu/gpu_texture.h"

void materials_setup(t_material_system *system)
{
	char path[PATH_MAX];
	{
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_colour_vertex.glsl");
		char *basic_vertex = strdup(path);
		
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_colour_fragment.glsl");
		char *basic_fragment = strdup(path);
		
		GLuint program;
		gpu_shader_program_compile_vert_frag(basic_vertex, basic_fragment, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "basic_colour_shader"});	
		t_material *basic = material_system_mat_create(system, (t_material){.shader = shader, .name = "basic_colour_material"});
		printf("Shader basic_colour_shader: %p\n", shader);
		// vec_mat_prop props = {};
		// vec_mat_prop_push(&props, (t_mat_prop){.name = "MVP", .type=MPT_MAT4});
		material_prop_add_new(basic, "MVP", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(basic, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});

		free(basic_fragment);
		free(basic_vertex);
	}

	{
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_lit_vertex.glsl");
		char *basic_vertex = strdup(path);
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_lit_fragment.glsl");
		char *basic_fragment = strdup(path);
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_lit_geo.glsl");
		char *basic_geo = strdup(path);

		GLuint program;
		gpu_shader_program_compile_vert_frag_geo(basic_vertex, basic_fragment,basic_geo, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "basic_lit_norm_shader"});	
		t_material *lit = material_system_mat_create(system, (t_material){.shader = shader, .name = "basic_lit_norm_material"});
		printf("Shader basic_lit_norm_shader: %p\n", shader);
		
		t_gpu_texture texture;
		gpu_texture_add(&texture, "crate", asset_get_path(path, 2, "textures", "uv.png"), 4);

		material_prop_add_new(lit, "diffuse_tex", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture.id, .slot = GL_TEXTURE0}});	

		material_prop_add_new(lit, "model", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "view", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "proj", MPT_MAT4, MPT_DEFAULT);

		material_prop_add_new(lit, "lightColour", MPT_FLOAT3, MPT_DEFAULT);
		material_prop_add_new(lit, "lightPos", MPT_FLOAT3, MPT_DEFAULT);
		material_prop_add_new(lit, "viewPos", MPT_FLOAT3, MPT_DEFAULT);

		material_prop_add_new(lit, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});
		
		free(basic_fragment);
		free(basic_vertex);
		free(basic_geo);
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
		printf("Shader normal_line_shader: %p\n", shader);

		material_prop_add_new(mat, "MVP", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(mat, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3 = {0, 1, 1}});

		free(vert);
		free(frag);
	}


	{
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_unlit_vertex.glsl");
		char *basic_vertex = strdup(path);
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_unlit_fragment.glsl");
		char *basic_fragment = strdup(path);

		GLuint program;
		gpu_shader_program_compile_vert_frag(basic_vertex, basic_fragment, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "basic_unlit_shader"});	
		t_material *unlit = material_system_mat_create(system, (t_material){.shader = shader, .name = "basic_unlit_material"});
		printf("Shader basic_unlit_shader: %p\n", shader);

		t_gpu_texture texture;
		gpu_texture_create(&texture, 1, 1, 0xFFFFFFFF);
		material_prop_add_new(unlit, "diffuse_tex", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture.id, .slot = GL_TEXTURE0}});

		material_prop_add_new(unlit, "model", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(unlit, "view", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(unlit, "proj", MPT_MAT4, MPT_DEFAULT);

		material_prop_add_new(unlit, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});
		
		free(basic_fragment);
		free(basic_vertex);
	}

	{
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_unlit_vertex.glsl");
		char *basic_vertex = strdup(path);
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_unlit_fragment.glsl");
		char *basic_fragment = strdup(path);

		GLuint program;
		gpu_shader_program_compile_vert_frag(basic_vertex, basic_fragment, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "basic_light_shader"});	
		t_material *unlit = material_system_mat_create(system, (t_material){.shader = shader, .name = "basic_light_material"});
		printf("Shader basic_light_shader: %p\n", shader);

		material_prop_add_new(unlit, "model", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(unlit, "view", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(unlit, "proj", MPT_MAT4, MPT_DEFAULT);
		
		free(basic_fragment);
		free(basic_vertex);
	}

	{
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_lit_vertex.glsl");
		char *basic_vertex = strdup(path);
		path[0] = '\0';
		asset_get_path(path, 2, "shaders", "basic_lit_fragment.glsl");
		char *basic_fragment = strdup(path);

		GLuint program;
		gpu_shader_program_compile_vert_frag(basic_vertex, basic_fragment, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "basic_lit_shader"});	
		t_material *lit = material_system_mat_create(system, (t_material){.shader = shader, .name = "basic_lit_material"});
		printf("Shader basic_lit_shader: %p\n", shader);
		

		t_gpu_texture texture;
		gpu_texture_add(&texture, "crate", asset_get_path(path, 2, "textures", "uv.png"), 4);

		material_prop_add_new(lit, "diffuse_tex", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture.id, .slot = GL_TEXTURE0}});	

		material_prop_add_new(lit, "model", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "view", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "proj", MPT_MAT4, MPT_DEFAULT);

		material_prop_add_new(lit, "lightColour", MPT_FLOAT3, MPT_DEFAULT);
		material_prop_add_new(lit, "lightPos", MPT_FLOAT3, MPT_DEFAULT);
		material_prop_add_new(lit, "viewPos", MPT_FLOAT3, MPT_DEFAULT);

		material_prop_add_new(lit, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});
		
		free(basic_fragment);
		free(basic_vertex);
	}

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
		t_material *lit = material_system_mat_create(system, (t_material){.shader = shader, .name = "general_lit_material"});
		printf("Shader general_lit_shader: %p\n", shader);
		t_gpu_texture texture;
		gpu_texture_add(&texture, "crate", asset_get_path(path, 2, "textures", "crate.png"), 4);

		t_gpu_texture texture_specular;
		gpu_texture_add(&texture_specular, "crate_spec", asset_get_path(path, 2, "textures", "crate_spec.png"), 4);

		material_prop_add_new(lit, "material.diffuse", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture.id, .slot = GL_TEXTURE0}});
		material_prop_add_new(lit, "material.specular", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture_specular.id, .slot = GL_TEXTURE1}});
		material_prop_add_new(lit, "material.shininess", MPT_FLOAT1, (t_mat_prop_value){.f1={64.0f}});

		material_prop_add_new(lit, "light.ambient", MPT_FLOAT3, (t_mat_prop_value){.f3={0.2, 0.2, 0.2}});
		material_prop_add_new(lit, "light.diffuse", MPT_FLOAT3, (t_mat_prop_value){.f3={0.5, 0.5, 0.5}});
		material_prop_add_new(lit, "light.specular", MPT_FLOAT3, (t_mat_prop_value){.f3={1.0, 1.0, 1.0}});
		material_prop_add_new(lit, "light.position", MPT_FLOAT3, (t_mat_prop_value){.f3={0.0, 0.0, 0.0}});

		material_prop_add_new(lit, "model", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "view", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(lit, "proj", MPT_MAT4, MPT_DEFAULT);

		material_prop_add_new(lit, "viewPos", MPT_FLOAT3, MPT_DEFAULT);

		material_prop_add_new(lit, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});
		
		free(general_fragment);
		free(general_vertex);
	}
	
}

static void setup_camera(t_engine* engine)
{
    engine->render_context.camera = CAMERA_DEFAULT;
	glm_vec3_copy((vec3){0,0,10}, engine->render_context.camera.transform.position);
	camera_init(&engine->render_context.camera, 70.0f, (float)engine->width / (float)engine->height, 0.1f, 100.0f);
}

static void setup_cubes(t_engine* engine, struct scene_cube_array* scene)
{
    char path[PATH_MAX];
    path[0] = '\0';

    scene->count = 1;
    scene->cubes = malloc(sizeof(t_entity*) * scene->count);

    // scene->cube_model = model_load(asset_get_path(path, 2, "models", ".no/cube_fwn.obj"));
    scene->cube_model = model_load(asset_get_path(path, 2, "models", "cube.obj"));
    scene->cube_gpu_mesh = gpu_mesh_add(&scene->cube_model->mesh);

	// engine->render_context.lightPos[0] = 10;
	// engine->render_context.lightPos[1] = 0;
	// engine->render_context.lightPos[2] = 10;

	// engine->render_context.lightColour[0] = 1;
	// engine->render_context.lightColour[1] = 1;
	// engine->render_context.lightColour[2] = 1;

    t_entity *light = scene->light = entity_create(ET_BASE);
    light->transform.position[0] = 1.2;
    light->transform.position[1] = 1.0;
    light->transform.position[2] = 2.0;

	light->transform.scale[0] = 0.1f;
    light->transform.scale[1] = 0.1f;
    light->transform.scale[2] = 0.1f;

	light->renderer = mesh_renderer_create(scene->cube_gpu_mesh, material_system_mat_get(&engine->render_context.material_system, "basic_light_material"));
	t_mat_prop* prop = material_prop_get(light->renderer->material, "colour");
	if (prop)
	{
		prop->value.f3[0] = 1;
		prop->value.f3[1] = 1;
		prop->value.f3[2] = 1;
		material_prop_update(light->renderer->material, prop);
	}
	entity_render_attach(&engine->render_context, light);

    // setup the cubes
    for (int i = 0; i < scene->count; i++)
    {
		int j = 0;
		// for (; j < scene->count; j++)
		{
			int k = 0;
			// for (; k < scene->count; k++)
			{
				t_entity *ent = entity_create(ET_BASE);
				ent->transform.position[0] = i * 3.5f;
				ent->transform.position[1] = j * 3.5f;
				ent->transform.position[2] = k * 3.5f;

				ent->transform.scale[0] = 1;
				ent->transform.scale[1] = 1;
				ent->transform.scale[2] = 1;
				t_material *material;
				// if (i % 2 == 0)
				// {
				// 	material = material_system_mat_get(&engine->render_context.material_system, "basic_lit_material");
				// }
				// else
				{
					material = material_system_mat_get(&engine->render_context.material_system, "general_lit_material");
				}
				scene->shared = material; 
				ent->renderer = mesh_renderer_create(scene->cube_gpu_mesh, material);
				entity_render_attach(&engine->render_context, ent);
			}
		}
    }


}


static e_engine_hook_result ca_render_thread(t_engine* engine, struct scene_cube_array* scene)
{
    const double delta_time = engine->delta_time;
    static float angle = 0.0;
    angle += delta_time;
    
    // float angleX = sin(angle) * 10;
    // float angleY = cos(angle * 0.5) * 10;
    // float angleZ = cos(angle) * 10;

    // engine->render_context.lightPos[0] = angleX;
    // engine->render_context.lightPos[1] = angleY;
    // engine->render_context.lightPos[2] = angleZ;
    // engine->render_context.lightColour[0] = 1;
    // engine->render_context.lightColour[1] = 0;
    // engine->render_context.lightColour[2] = 1;
	// scene->light->transform.position[0] = angleX;
	// scene->light->transform.position[1] = angleY;
	// scene->light->transform.position[2] = angleZ;

	
	vec3 ambientColour;
	glm_vec3_copy((vec3){0.2f, 0.2f, 0.2f}, ambientColour);
	vec3 diffuseColor;
	glm_vec3_copy((vec3){0.5f, 0.5f, 0.5f}, diffuseColor);

	material_prop_update_named(scene->shared, "light.ambient", (t_mat_prop_value){.f3={ambientColour[0], ambientColour[1], ambientColour[2]}});
	material_prop_update_named(scene->shared, "light.diffuse", (t_mat_prop_value){.f3={diffuseColor[0], diffuseColor[1], diffuseColor[2]}});
	material_prop_update_named(scene->shared, "light.specular", (t_mat_prop_value){.f3={1.0f, 1.0f, 1.0f}});

	// material properties
	//material_prop_update_named(scene->shared, "material.diffuse", (t_mat_prop_value){.f3={1.0f, 0.5f, 0.31f}});
	//material_prop_update_named(scene->shared, "material.specular", (t_mat_prop_value){.f3={0.5f, 0.5f, 0.5f}}); // specular lighting doesn't have full effect on this object's material
	material_prop_update_named(scene->shared, "material.shininess", (t_mat_prop_value){.f1={64.0f}});
	material_prop_update_named(scene->shared, "light.position", 
		(t_mat_prop_value){.f3={scene->light->transform.position[0],
								scene->light->transform.position[1],
								scene->light->transform.position[2]}});

	// printf("ca_render_thread: %f\n", engine->delta_time);
    return ENGINE_HOOK_RESULT_CONTINUE;
}
#include "logging.h"
static e_engine_hook_result scene_start(t_engine* engine, struct scene_cube_array* scene)
{
	te_logf(LOG_LEVEL_INFO, "scene", "Starting scene cube_array");
    setup_cubes(engine, scene);
    return ENGINE_HOOK_RESULT_CONTINUE;
}

static e_engine_hook_result scene_awake(t_engine* engine, struct scene_cube_array* scene)
{
    setup_camera(engine);
    materials_setup(&engine->render_context.material_system);

    return ENGINE_HOOK_RESULT_CONTINUE;
}

e_engine_hook_result scene_setup_cube_array(t_engine* engine, void* context)
{
    struct scene_cube_array* scene = malloc(sizeof(*scene));
    *scene = (struct scene_cube_array){0};

    engine->posthook = (void*)&scene_awake;
    engine->hook_context = scene;

    engine->scene = (void*)&scene_start;
    engine->scene_context = scene;

    engine->render_thread_frame_interhook = (void*)&ca_render_thread;
    engine->render_thread_context = scene;
    return ENGINE_HOOK_RESULT_CONTINUE;
}
