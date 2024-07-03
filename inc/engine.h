#ifndef ENGINE_H
#define ENGINE_H

#include "material_system.h"
#include "renderer.h"

typedef struct 
{
	t_material_system material;
	t_render_ctx render_context;
} t_engine;

#endif