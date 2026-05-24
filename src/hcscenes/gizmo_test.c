#include "engine.h"
#include "io.h"
#include "model.h"
#include "asset.h"
#include "entity.h"
#include "renderer.h"
#include "entity/light.h"

struct scene_gizmo_test
{
    int unused;
};

#include "material_system.h"
#include "lighting.h"
#include "io.h"
#include "util.h"
#include "random.h"
#include "gizmo.h"

static void setup_camera(t_engine* engine)
{
    engine->render_context.camera = CAMERA_DEFAULT;
	glm_vec3_copy((vec3){0,0,10}, engine->render_context.camera.transform.position);
	camera_init(&engine->render_context.camera, 70.0f, (float)engine->width / (float)engine->height, 0.1f, 100.0f);
}


static void setup_materials(t_material_system *system)
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

		// vec_mat_prop props = {};
		// vec_mat_prop_push(&props, (t_mat_prop){.name = "MVP", .type=MPT_MAT4});
		material_prop_add_new(basic, "MVP", MPT_MAT4, MPT_DEFAULT);
		material_prop_add_new(basic, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});

		free(basic_fragment);
		free(basic_vertex);
	}
}

static e_engine_hook_result ca_render_thread(t_engine* engine, void* scene_ptr)
{
	struct scene_gizmo_test* const scene = (struct scene_gizmo_test*)scene_ptr;

    const double delta_time = engine->delta_time;
    static float angle = 0.0;
    angle += delta_time;

    vec3 line_end = {cosf(angle) * 5.0f, 0, sinf(angle) * 5.0f};
    gizmo_line(engine, (vec3){0,0,0}, line_end, (vec3){1,0,0}, false);

    return ENGINE_HOOK_RESULT_CONTINUE;
}

#include "logging.h"
#include "vector4.h"

static e_engine_hook_result scene_start(t_engine* engine, void* scene_ptr)
{
	struct scene_gizmo_test* const scene = (struct scene_gizmo_test*)scene_ptr;
	te_logf(LOG_LEVEL_INFO, "scene", "Starting scene gizmo_test");

	engine->clear_colour = (t_vec4){.v = {0.05, 0.05, 0.05, 0.05}};
    return ENGINE_HOOK_RESULT_CONTINUE;
}

static e_engine_hook_result scene_awake(t_engine* engine, void* scene_ptr)
{
    UNUSED(scene_ptr);
    setup_camera(engine);
    setup_materials(&engine->render_context.material_system);
    gizmo_line(engine, (vec3){0,0,0}, (vec3){1,0,0}, (vec3){1,0,0}, true);
    gizmo_cube(engine, (vec3){0,0,0}, (vec3){1,1,1}, (vec3){1,0,0}, true);
    return ENGINE_HOOK_RESULT_CONTINUE;
}

e_engine_hook_result scene_setup_gizmo_test(t_engine* engine, void* context)
{
	UNUSED(context);
    struct scene_gizmo_test* scene = malloc(sizeof(*scene));
    *scene = (struct scene_gizmo_test){0};

    engine->posthook = (void*)&scene_awake;
    engine->hook_context = scene;

    engine->scene = (void*)&scene_start;
    engine->scene_context = scene;

    engine->render_thread_frame_interhook = (void*)&ca_render_thread;
    engine->render_thread_context = scene;
    return ENGINE_HOOK_RESULT_CONTINUE;
}
