#ifndef ENGINE_H
#define ENGINE_H

#include "material_system.h"
#include "renderer.h"
#include "input.h"
#include "vector4.h"

typedef struct s_engine t_engine;
typedef enum e_engine_hook_result {
	ENGINE_HOOK_RESULT_CONTINUE,
	ENGINE_HOOK_RESULT_OVERRIDE,
	ENGINE_HOOK_RESULT_ERROR
} e_engine_hook_result;

// Called before the engine initializes OpenGL, creates the window, and sets up the render context.
// Assume no systems are initialised.
typedef e_engine_hook_result (*t_engine_prehook_func)(t_engine* engine, void* context);

// prehook should setup materials, load models
typedef e_engine_hook_result (*t_engine_posthook_func)(t_engine* engine, void* context);

// posthook should setup the scene, entities, and renderers
typedef e_engine_hook_result (*t_engine_scene_func)(t_engine* engine, void* context);

// Called every frame.
typedef e_engine_hook_result (*t_engine_render_thread_func)(t_engine* engine, void* context);


#define ENGINE_HAS_CONTROL_FLAG(engine, flag) (((engine->control_flags) & flag) == (flag))

enum e_engine_control_flags
{
    ENGINE_CONTROL_RENDER_DO_CLEAR_COLOUR = 1 << 0,
    ENGINE_CONTROL_RENDER_DO_CLEAR_DEPTH = 1 << 1,
    ENGINE_CONTROL_RENDER_DO_DEFAULT_CAMERA_UPDATE = 1 << 2,
	ENGINE_CONTROL_RENDER_BACK_FACE_CULLING = 1 << 3,
	ENGINE_CONTROL_DEFAULT = 	ENGINE_CONTROL_RENDER_DO_CLEAR_COLOUR | 
								ENGINE_CONTROL_RENDER_DO_CLEAR_DEPTH | 
								ENGINE_CONTROL_RENDER_DO_DEFAULT_CAMERA_UPDATE | 
								ENGINE_CONTROL_RENDER_BACK_FACE_CULLING,

};

struct s_engine
{
	GLFWwindow* window;

	enum e_engine_control_flags control_flags;
	int width;
	int height;

    t_inputctx input;

	t_render_ctx render_context;
	t_vec4 clear_colour;

	uint32_t target_fps;

	bool multi_thread_render;

	double delta_time;
	t_engine_prehook_func prehook;
	void* prehook_context;

	t_engine_posthook_func posthook;
	void* hook_context;

	t_engine_scene_func scene;
	void* scene_context;

	t_engine_render_thread_func render_thread_setup;
	t_engine_render_thread_func render_thread_frame_prehook;
	t_engine_render_thread_func render_thread_frame_interhook;
	t_engine_render_thread_func render_thread_frame_posthook;
	void* render_thread_context;
};

#endif