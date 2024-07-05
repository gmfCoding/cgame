#ifndef ENGINE_H
#define ENGINE_H

#include "material_system.h"
#include "renderer.h"
#include "input.h"

typedef struct 
{
	GLFWwindow* window;
    t_inputctx input;

	t_material_system material;
	t_render_ctx render_context;

	uint32_t target_fps;
} t_engine;

#endif