
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
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#include <GL/glx.h>

#include "core/procmesh.h"
#include "transform.h"

int engine_setup(t_engine *engine)
{
	engine->control_flags = ENGINE_CONTROL_DEFAULT;
	if (engine->prehook && engine->prehook(engine, engine->prehook_context) != ENGINE_HOOK_RESULT_CONTINUE)
		return -1;
	/* Initialize the library */
	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	//engine->window = glfwCreateWindow(640, 480, "Hell	o World", NULL, NULL);
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
	glfwSetInputMode(engine->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // GLFW_CURSOR_DISABLED
	glfwSetCursorPos(engine->window, 0, 0);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		perror("Failed to initialize GLAD");
		return -1;
	}
	glfwShowWindow(engine->window);
	if (engine->posthook && engine->posthook(engine, engine->hook_context) != ENGINE_HOOK_RESULT_CONTINUE)
		return -1;
	return 0;
}


// #include "renderdoc_app.h"
// #include <dlfcn.h>  // Linux/Mac
// // #include <windows.h>  // Windows
// #include <errno.h>

// RENDERDOC_API_1_4_2 *rdoc_api = NULL;

// void init_renderdoc() {
//     // Load the RenderDoc library
// 	// RTLD_GLOBAL is required so RenderDoc's hooked GL symbols override the real ones
// 	void *mod = dlopen("/home/clovell/Downloads/renderdoc_1.43/lib/librenderdoc.so", RTLD_NOW | RTLD_GLOBAL);
// 	if (mod == NULL)
// 	{
		
// 		printf("Cannot locate librenderdoc.so: %s\n", strerror(errno));
// 	}
// 	pRENDERDOC_GetAPI RENDERDOC_GetAPI = 
// 		(pRENDERDOC_GetAPI)dlsym(mod, "RENDERDOC_GetAPI");

//     // Get the API
//     int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_4_2, (void **)&rdoc_api);
//     if(ret != 1) {
//         printf("Failed to load RenderDoc API\n");
//         return;
//     }
    
//     printf("RenderDoc API loaded successfully\n");
// }

// void start_frame_capture(t_engine *engine) {
//     if(rdoc_api) {
//         rdoc_api->StartFrameCapture(NULL, NULL);
// 	}
// }


// void end_frame_capture(t_engine *engine) {
//     if(rdoc_api) {
//         if (rdoc_api->EndFrameCapture(NULL, NULL) == 1)
// 			printf("Creating capture at: %s\n", rdoc_api->GetCaptureFilePathTemplate());
// 		else
// 			printf("Capture Failed!\n");
// 	}
// }

// void set_capture_file_path(const char *path) {
//     if(rdoc_api) {
//         rdoc_api->SetCaptureFilePathTemplate(path);
//     }
// }

static void render_thread_preframe(t_engine *engine)
{
	int flags = 0;
	if (ENGINE_HAS_CONTROL_FLAG(engine, ENGINE_CONTROL_RENDER_DO_CLEAR_COLOUR))
	{
		GLCall(glClearColor(.2,.2,.6,1));
		flags |= GL_COLOR_BUFFER_BIT;
	}
	if (ENGINE_HAS_CONTROL_FLAG(engine, ENGINE_CONTROL_RENDER_DO_CLEAR_DEPTH))
		flags |= GL_DEPTH_BUFFER_BIT;
	if (ENGINE_HAS_CONTROL_FLAG(engine, ENGINE_CONTROL_RENDER_DO_CLEAR_COLOUR | ENGINE_CONTROL_RENDER_DO_CLEAR_DEPTH))
		glClear(flags);
}

static void render_thread_camera(t_engine *engine, float delta_time)
{
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
}

int render_thread(t_engine *engine)
{
	char path[PATH_MAX];

	glfwMakeContextCurrent(engine->window);

	// volatile int loopme = 1;
	// while (loopme)
	// {
	// 	glfwWaitEventsTimeout(0.1); 
	// 	if (glfwGetKey(engine->window, GLFW_KEY_F12) == GLFW_PRESS)
	// 		loopme = 0;
	// }

	double delta_time = 0;
	double time_last_frame = 0;
	double lasttime = 0;
	//set_capture_file_path("./captures/frame");

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(engine->window))
	{
		// static bool capture_started = false;
		// if (input_keydown(&engine->input, KEY_F12))
		// {
		// 	printf("Starting Capture!\n");
		// 	start_frame_capture(engine);
		// 	capture_started = true;
		// }	

		delta_time = glfwGetTime() - time_last_frame;
		time_last_frame = glfwGetTime();
		engine->delta_time = delta_time;

		if (engine->render_thread_frame_prehook && engine->render_thread_frame_prehook(engine, engine->render_thread_context) != ENGINE_HOOK_RESULT_CONTINUE)
			return -1;
		render_thread_preframe(engine);

		if (ENGINE_HAS_CONTROL_FLAG(engine, ENGINE_CONTROL_RENDER_DO_DEFAULT_CAMERA_UPDATE))
			render_thread_camera(engine, delta_time);

		if (ENGINE_HAS_CONTROL_FLAG(engine, ENGINE_CONTROL_RENDER_BACK_FACE_CULLING))
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);


		if (engine->render_thread_frame_interhook && engine->render_thread_frame_interhook(engine, engine->render_thread_context) != ENGINE_HOOK_RESULT_CONTINUE)
			return -1;
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// t_transform transform = TTRANSFORM_IDENTITY;
		// glm_euler_xyz_quat((vec3){90, 0, 0}, transform.rotation);
		//render_mesh_renderer(&engine->render_context, &transform, grid_renderer);

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
		engine->render_context.active_shader = NULL;
		// if (capture_started == true)
		// {
		// 	printf("aoisdhiasud\n");
		// 	end_frame_capture(engine);
		// 	capture_started = false;
		// }
	}
	return 0;
}

#include "hcscenes/scenes.h"

void scene_select(t_engine* engine, char* name)
{
	if (strncmp(name, "cubes", 10) == 0)
		engine->prehook = scene_setup_cube_array;
	// else if (strncmp(name, "model_viewer", 12) == 0)
	// 	engine->prehook = scene_setup_model_viewer;	
	// else if (strncmp(name, "procmesh", 8) == 0)
	// 	engine->prehook = scene_setup_procmesh;
	// else if (strncmp(name, "instancing", 10) == 0)
	// 	engine->prehook = scene_setup_instancing;
	// else if (strncmp(name, "example", 14) == 0)
	// 	engine->prehook = scene_setup_example;
}

int main(int argc, char** argv)
{
	t_engine engine = {0};
	engine.target_fps = 144;
	engine.input = (t_inputctx){0};
	glfw_input_context = &engine.input;

	// init_renderdoc();

	if (argc >= 2)
		scene_select(&engine, argv[1]);
	else
		scene_select(&engine, "cubes");

	if (engine_setup(&engine) != 0)
		return -1;

	if (engine.scene && engine.scene(&engine, engine.scene_context) != ENGINE_HOOK_RESULT_CONTINUE)
		return -1;

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
