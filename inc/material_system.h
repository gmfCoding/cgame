#ifndef MATERIAL_SYSTEM_H
# define MATERIAL_SYSTEM_H

#include "cont/map_str_material.h"
#include "cont/map_str_shader.h"

typedef struct s_material_system
{
	map_str_material materials;
	map_str_shader shaders;

} t_material_system;

void material_system_init(t_material_system *system);;

t_material *material_system_mat_get(t_material_system *system, const char* name);

t_shader *material_system_shader_get(t_material_system *system, const char* name);

t_material *material_system_mat_create(t_material_system *system, t_material material);

t_shader *material_system_shader_add(t_material_system *system, t_shader shader);

#endif