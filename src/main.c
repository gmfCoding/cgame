
#include "cglm/euler.h"
#include "cglm/quat.h"
#include "cglm/vec3.h"
#include "load_glad.h"
#include <GLFW/glfw3.h>
#include <string.h>
#include <pthread.h>


#include "io.h"
#include "asset.h"

#include <string.h>
#include <stdio.h>
#include "shader.h"
#include "model.h"
#include "gldebug.h"
#include "gpu/mesh.h"
#include "gpu/gpu_texture.h"
#include "texture.h"

#include "material.h"
#include "material_system.h"
#include "engine.h"
#include "entity.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"

#include "core/procmesh.h"
#include "transform.h"

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
		
		GLuint program;
		gpu_shader_program_compile_vert_frag(basic_vertex, basic_fragment, &program);
		t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "basic_lit_shader"});	
		t_material *lit = material_system_mat_create(system, (t_material){.shader = shader, .name = "basic_lit_material"});
		
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
}

int engine_setup(t_engine *engine)
{
/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	//engine->window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	GLFWmonitor* mon = NULL;
	if (false)
		mon = glfwGetPrimaryMonitor();

	//glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
	// const GLFWvidmode * mode = glfwGetVideoMode(mon);
	// glfwInitHint(GLFW_RESIZABLE, GLFW_RESIZE_ALL_CURSOR);
	engine->width = 1920;
	engine->height = 1080;
	engine->window = glfwCreateWindow(engine->width, engine->height, "My Title", mon, NULL);
	if (!engine->window)
	{
		glfwTerminate();
		return -1;
	}
	glfwWaitEventsTimeout(0.1); 
	/* Make the window's context current */
	glfwMakeContextCurrent(engine->window);
	glfwSwapInterval(0);
	glfwSetKeyCallback(engine->window, input_cb_key);
	engine->input.first_time = true;

	glfwSetCursorPosCallback(engine->window, input_cb_mouse_move);

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(engine->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetInputMode(engine->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(engine->window, 0, 0);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		perror("Failed to initialize GLAD");
		return -1;
	}
	glfwShowWindow(engine->window);
	return 0;
}

int render_thread(t_engine *engine)
{
	char path[PATH_MAX];

	glfwMakeContextCurrent(engine->window);
	t_model *crate = model_load(asset_get_path(path, 2, "models", "triangle.obj"));
	//t_gpu_texture* texture = gpu_texture_add("unknown", );
	t_gpu_mesh *mesh = gpu_mesh_add(&crate->mesh); 
	(void)mesh;
	
	t_entity *ent = entity_create(ET_BASE);
	ent->renderer = mesh_renderer_create(mesh, material_system_mat_get(&engine->material, "basic_lit_material"));
	entity_render_attach(&engine->render_context, ent);

	t_pm_grid grid = {0};
	pm_grid_create(&grid, 10, 10, true);
	t_mesh_renderer *grid_renderer = mesh_renderer_create(grid.gpu_mesh, material_system_mat_get(&engine->material, "basic_lit_material"));

	t_mat_prop *lightPos = material_prop_get(ent->renderer->material, "lightPos");
	t_mat_prop *lightColour = material_prop_get(ent->renderer->material, "lightColour");
	lightPos->value = (t_mat_prop_value){.f3 = {10, 0, 10}};
	lightColour->value = (t_mat_prop_value){.f3 = {1, 0, 1}};

	double delta_time = 0;
	double time_last_frame = 0;
	double lasttime = 0;


	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(engine->window))
	{
		delta_time = glfwGetTime() - time_last_frame;
		time_last_frame = glfwGetTime();
		GLCall(glClearColor(.2,.2,.6,1));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera_view_update(&engine->render_context.camera);

		t_move move = {0};

		move.Forward = input_keyheld(&engine->input, KEY_W);
		move.Backwards = input_keyheld(&engine->input, KEY_S);
		move.Left = input_keyheld(&engine->input, KEY_A);
		move.Right = input_keyheld(&engine->input, KEY_D);
		move.Up = input_keyheld(&engine->input, KEY_E);
		move.Down = input_keyheld(&engine->input, KEY_Q);

		camera_control_look(&engine->render_context.camera, &engine->input);
		camera_control(&engine->render_context.camera, &move, delta_time);

		static float angle = 0.0;
		angle += delta_time;
		
		float angleX = sin(angle) * 10;
		float angleY = cos(angle * 0.5) * 10;
		float angleZ = cos(angle) * 10;

		t_mat_prop *lightPos = material_prop_get(ent->renderer->material, "lightPos");
		lightPos->value = (t_mat_prop_value){.f3 = {angleX, angleY, angleZ}};
		material_prop_update(ent->renderer->material, lightPos);
		
		// t_mat_prop *lightPos2 = material_prop_get(grid_renderer->material, "lightPos");
		// lightPos2->value = (t_mat_prop_value){.f3 = {angleX, angleY, angleZ}};
		// material_prop_update(grid_renderer->material, lightPos2);

		//glDisable(GL_CULL_FACE);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		t_transform transform = TTRANSFORM_IDENTITY;
		glm_euler_xyz_quat((vec3){0, 0, 0}, transform.rotation);
		render_mesh_renderer(&engine->render_context, &transform, grid_renderer);

		render_entities(&engine->render_context);
		input_process(&engine->input);
		input_mouse_move_end(&engine->input);
		glfwSwapBuffers(engine->window);

		while (engine->target_fps > 0 && glfwGetTime() < lasttime + 1.0 / engine->target_fps) {
			// TODO: Put the thread to sleep, yield, or simply do nothing
		}
		// printf("dt:%f\n", 1/delta_time);
		lasttime += 1.0 / engine->target_fps;
		if(!engine->multi_thread_render)
			glfwPollEvents();
	}
	return 0;
}

int main(void)
{
	t_engine engine = {0};
	engine.target_fps = 144;
	engine.input = (t_inputctx){0};
	glfw_input_context = &engine.input;

	if (engine_setup(&engine) != 0)
		return -1;

	engine.render_context.camera = CAMERA_DEFAULT;
	glm_vec3_copy((vec3){0,0,10}, engine.render_context.camera.transform.position);
	camera_init(&engine.render_context.camera, 70.0f, (float)engine.width / (float)engine.height, 0.1f, 100.0f);
	materials_setup(&engine.material);
	glfwMakeContextCurrent(engine.window);
	if (engine.multi_thread_render)
	{
		pthread_t render_thread_id;
		pthread_create(&render_thread_id, NULL, (void *(*)(void *))render_thread, &engine);
		while (!glfwWindowShouldClose(engine.window)) {
			glfwPollEvents();
		}
		pthread_join(render_thread_id, NULL);
	}
	else {
		render_thread(&engine);
	}
	glfwTerminate();
	return 0;
}
