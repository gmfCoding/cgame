#include "material_system.h"


void material_system_init(t_material_system *system)
{
	system->materials = (map_str_material){0};
	system->shaders = (map_str_shader){0};
}

t_material *material_system_mat_get(t_material_system *system, const char* name)
{
	return &map_str_material_get_mut(&system->materials, name)->second;
}

t_shader *material_system_shader_get(t_material_system *system, const char* name)
{
	return &map_str_shader_get_mut(&system->shaders, name)->second;
}

t_material *material_system_mat_create(t_material_system *system, t_material material)
{
	map_str_material_emplace(&system->materials, material.name, material);
	return &map_str_material_get_mut(&system->materials, material.name)->second;
}

t_shader *material_system_shader_add(t_material_system *system, t_shader shader)
{
	map_str_shader_emplace(&system->shaders, shader.name, shader);
	return &map_str_shader_get_mut(&system->shaders, shader.name)->second;
}