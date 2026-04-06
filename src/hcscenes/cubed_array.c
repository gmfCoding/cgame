#include "engine.h"
#include "io.h"
#include "model.h"
#include "asset.h"
#include "entity.h"
#include "renderer.h"
#include "entity/light.h"

struct scene_cube_array
{
    t_entity** cubes;
    int count;
    t_model* cube_model;
    t_gpu_mesh* cube_gpu_mesh;
    t_light *light;
    t_light *lights;
	t_material* shared;
};

#include "material_system.h"
#include "lighting.h"
#include "io.h"
#include "gpu/gpu_texture.h"
#include "util.h"
#include "random.h"

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

		// t_gpu_texture texture;
		// gpu_texture_create(&texture, 1, 1, 0xFFFFFFFF);
		// material_prop_add_new(unlit, "diffuse_tex", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture.id, .slot = GL_TEXTURE0}});

		material_prop_add_new(unlit, "model", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(unlit, "view", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(unlit, "proj", MPT_MAT4, MPT_DEFAULT);

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
		material_prop_add_new(unlit, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});
		
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

		material_prop_add_new(lit, "material.diffuse", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture.id, .slot = 0}});
		material_prop_add_new(lit, "material.specular", MPT_SAMPLER2D, (t_mat_prop_value){.texslot = {.tex = texture_specular.id, .slot = 1}});
		material_prop_add_new(lit, "material.shininess", MPT_FLOAT1, (t_mat_prop_value){.f1={64.0f}});
		material_prop_add_new(lit, "bliggy", MPT_FLOAT1, (t_mat_prop_value){.f1={64.0f}});

		material_prop_add_new_light(LT_DIRECTION, 0, lit);
		lit->max_lights = 1;
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

    // setup the cubes
    for (int i = 0; i < scene->count; i++)
    {
		int j = 0;
		for (; j < scene->count; j++)
		{
			int k = 0;
			for (; k < scene->count; k++)
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

		// engine->render_context.lightPos[0] = 10;
	// engine->render_context.lightPos[1] = 0;
	// engine->render_context.lightPos[2] = 10;

	// engine->render_context.lightColour[0] = 1;
	// engine->render_context.lightColour[1] = 1;
	// engine->render_context.lightColour[2] = 1;
    scene->lights = malloc(sizeof(t_light) * scene->shared->max_lights);

	t_rand rand = {0};
	msrand(&rand, 4);

	t_vec3 lightColours[] = {
		v3new(0, 0, 1),
		v3new(0, 1, 0),
		v3new(1, 0, 0),
		v3new(0, 0, 1),
		v3new(0, 1, 0),
		v3new(1, 0, 0),
		v3new(0, 0, 1),
		v3new(0, 1, 0),
		v3new(1, 0, 0),
		v3new(0, 0, 1),
		v3new(0, 1, 0),
		v3new(1, 0, 0),
		v3new(0, 0, 1),
		v3new(0, 1, 0),
		v3new(1, 0, 0),
		v3new(0, 0, 1),
		v3new(0, 1, 0),
		v3new(1, 0, 0),
	};

	for (int i = 0; i < scene->shared->max_lights; i++)
	{
		t_light *light = (t_light*)entity_create_unowned(ET_LIGHT, (void*)&scene->lights[i]);
		light->light.type = LT_POINT;
		light->light.point.index = i;
		light->light.point.constant = 1.0f;
		light->light.point.linear = 0.022f;
		light->light.point.quadratic = 0.0019f;

		light->base.transform.scale[0] = 0.1f;
		light->base.transform.scale[1] = 0.1f;
		light->base.transform.scale[2] = 0.1f;

		light->light.diffuse.v[0] = lightColours[i].x;
		light->light.diffuse.v[1] = lightColours[i].y;
		light->light.diffuse.v[2] = lightColours[i].z;
		glm_vec3_scale(light->light.diffuse.v, 0.4, light->light.ambient.v);
		char test[] = "light_material__";
		test[sizeof(test)-3] = i + '0';
		light->base.renderer = mesh_renderer_create(scene->cube_gpu_mesh, material_system_mat_dup(&engine->render_context.material_system, material_system_mat_get(&engine->render_context.material_system, "basic_light_material"), strdup(test)));
		t_mat_prop* prop = material_prop_get(light->base.renderer->material, "colour");
		if (prop)
		{
			glm_vec3_copy(light->light.diffuse.v, prop->value.f3);
			material_prop_update(light->base.renderer->material, prop);
			printf("updating mat for light: %d (%f %f %f) @ %p\n", i, prop->value.f3[0], prop->value.f3[1], prop->value.f3[2], light->base.renderer->material);
		}
		entity_render_attach(&engine->render_context, &light->base);
	}

	material_prop_update_named(scene->shared, "bliggy", (t_mat_prop_value){.f1={64.0f}});

}


static e_engine_hook_result ca_render_thread(t_engine* engine, struct scene_cube_array* scene)
{
    const double delta_time = engine->delta_time;
    static float angle = 0.0;
    angle += delta_time;
    
	for (int i = 0; i < scene->shared->max_lights; i++)
	{
		float angleX = sin(angle + i) * 10;
		float angleY = tan(cos((angle + i) * 0.5) * 10);
		float angleZ = cos(angle + i) * 10;
		
		scene->lights[i].base.transform.position[0] = angleX;
		scene->lights[i].base.transform.position[1] = angleY;
		scene->lights[i].base.transform.position[2] = angleZ;
		material_light_update(scene->shared, &scene->lights[i]);
	}

	t_light test = {0};
	test.light.type = LT_DIRECTION;
	test.light.directional.direction = v3new(-0.2f, -1.0f, -0.3f);
	test.light.ambient = v3new(0.2f, 0.2f, 0.2f);
	test.light.diffuse = v3new(0.4f, 0.4f, 0.4f);
	test.light.specular = v3new(1.0f, 1.0f, 1.0f);
	material_light_update(scene->shared, &test);


	//material_prop_update_named(scene->shared, "material.diffuse", (t_mat_prop_value){.f3={1.0f, 0.5f, 0.31f}});
	//material_prop_update_named(scene->shared, "material.specular", (t_mat_prop_value){.f3={0.5f, 0.5f, 0.5f}}); // specular lighting doesn't have full effect on this object's material
	material_prop_update_named(scene->shared, "material.shininess", (t_mat_prop_value){.f1={64.0f}});

	// printf("ca_render_thread: %f\n", engine->delta_time);
    return ENGINE_HOOK_RESULT_CONTINUE;
}
#include "logging.h"
#include "vector4.h"

static e_engine_hook_result scene_start(t_engine* engine, struct scene_cube_array* scene)
{
	te_logf(LOG_LEVEL_INFO, "scene", "Starting scene cube_array");
    setup_cubes(engine, scene);
	engine->clear_colour = (t_vec4){.v = {0.05, 0.05, 0.05, 0.05}};
    return ENGINE_HOOK_RESULT_CONTINUE;
}

static e_engine_hook_result scene_awake(t_engine* engine, struct scene_cube_array* scene)
{
	UNUSED(scene);
    setup_camera(engine);
    materials_setup(&engine->render_context.material_system);

    return ENGINE_HOOK_RESULT_CONTINUE;
}

e_engine_hook_result scene_setup_cube_array(t_engine* engine, void* context)
{
	UNUSED(context);
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
