#include "load_glad.h"
#include <GLFW/glfw3.h>
#include "io.h"
#include "asset.h"

#include <string.h>
#include <stdio.h>
#include "shader.h"
#include "model.h"
#include "gldebug.h"
#include "gpu/mesh.h"
#include "material.h"
#include "material_system.h"
#include "engine.h"
#include "entity.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"

void materials_setup(t_material_system *system)
{
    char path[PATH_MAX];
    
    path[0] = '\0';
    asset_get_path(path, 2, "shaders", "basic_vertex.glsl");
    char *basic_vertex = strdup(path);
    path[0] = '\0';
    asset_get_path(path, 2, "shaders", "basic_fragment.glsl");
    char *basic_fragment = strdup(path);

	GLuint program;
    gpu_shader_program_compile_vert_frag(basic_vertex, basic_fragment, &program);
	t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "basic_shader"});	
	t_material *basic = material_system_mat_create(system, (t_material){.shader = shader, .name = "basic_material"});

	vec_mat_prop props = {};
	vec_mat_prop_push(&props, (t_mat_prop){.name = "MVP", .type=MPT_MAT4});
	material_prop_add_new(basic, "MVP", MPT_MAT4, MPT_DEFAULT);
	material_prop_add_new(basic, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});

	free(basic_fragment);
	free(basic_vertex);
}

int engine_setup(t_engine *engine)
{
/* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    engine->window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!engine->window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(engine->window);

    glfwSetKeyCallback(engine->window, input_cb_key);
    engine->input.first_time = true;

    glfwSetCursorPosCallback(engine->window, input_cb_mouse_move);

	if (glfwRawMouseMotionSupported())
    	glfwSetInputMode(engine->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetInputMode(engine->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        perror("Failed to initialize GLAD");
        return -1;
    }
	return 0;
}

int main(void)
{
	t_engine engine = {0};
	engine.target_fps = 25;
	engine.input = (t_inputctx){0};
    glfw_input_context = &engine.input;

	if (engine_setup(&engine) != 0)
		return -1;

    engine.render_context.camera = CAMERA_DEFAULT;
    glm_vec3_copy((vec3){0,0,10}, engine.render_context.camera.transform.position);
	camera_init(&engine.render_context.camera, 70.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    materials_setup(&engine.material);

    char path[PATH_MAX];
    t_model *crate = model_load(asset_get_path(path, 2, "models", "crate.obj"));
	// t_gpu_texture* texture = gpu_texture_add(t_texture *texture);
    t_gpu_mesh *mesh = gpu_mesh_add(&crate->mesh); 
    (void)mesh;
	
	t_entity *ent = entity_create(ET_BASE);
	ent->renderer = mesh_renderer_create(mesh, material_system_mat_get(&engine.material, "basic_material"));
	entity_render_attach(&engine.render_context, ent);
	double delta_time = 0;
	double time_last_frame = 0;
	double lasttime = 0;
    while (!glfwWindowShouldClose(engine.window))
    {
		delta_time = glfwGetTime() - time_last_frame;
		time_last_frame = glfwGetTime();
        GLCall(glClearColor(.2,.2,.6,1));
        glClear(GL_COLOR_BUFFER_BIT);
        camera_view_update(&engine.render_context.camera);

        t_move move = {0};

        move.Forward = input_keyheld(&engine.input, KEY_W);
        move.Backwards = input_keyheld(&engine.input, KEY_S);
        move.Left = input_keyheld(&engine.input, KEY_A);
        move.Right = input_keyheld(&engine.input, KEY_D);
        move.Up = input_keyheld(&engine.input, KEY_E);
        move.Down = input_keyheld(&engine.input, KEY_Q);

        camera_control_look(&engine.render_context.camera, &engine.input);
        camera_control(&engine.render_context.camera, &move, delta_time);
		render_entities(&engine.render_context);
        input_process(&engine.input);
        input_mouse_move_end(&engine.input);
        glfwSwapBuffers(engine.window);


		while (engine.target_fps > 0 && glfwGetTime() < lasttime + 1.0 / engine.target_fps) {
            // TODO: Put the thread to sleep, yield, or simply do nothing
        }

        lasttime += 1.0 / engine.target_fps;
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}