#include "material_system.h"
#include "logging.h"

#define FMT_MAT_NOT_FOUND "Material '%s' not found in material system"
#define FMT_SHD_NOT_FOUND "Shader '%s' not found in material system"

void material_system_init(t_material_system *system)
{
	system->materials = (map_str_material){0};
	system->shaders = (map_str_shader){0};
}

t_material *material_system_mat_dup(t_material_system *system, t_material* material, const char* new_name)
{
	t_material new_mat = *material;
	new_mat.name = new_name;
	new_mat.prop_names = set_str_clone(material->prop_names);
	new_mat.properties = map_mat_prop_clone(material->properties);
	return material_system_mat_create(system, new_mat);
}

t_material *material_system_mat_get(t_material_system *system, const char* name)
{
	if (map_str_material_contains(&system->materials, name) == false)
		return te_logf(LOG_LEVEL_ERROR, "material", FMT_MAT_NOT_FOUND, name), NULL;
	t_material* mat = map_str_material_get(&system->materials, name)->second;
	if (mat == NULL)
		te_logf(LOG_LEVEL_ERROR, "material", FMT_MAT_NOT_FOUND, name);
	return mat;
}

t_shader *material_system_shader_get(t_material_system *system, const char* name)
{
	if (map_str_shader_contains(&system->shaders, name) == false)
		return te_logf(LOG_LEVEL_ERROR, "material", FMT_SHD_NOT_FOUND, name), NULL;
	return map_str_shader_get(&system->shaders, name)->second;
}

t_material *material_system_mat_create(t_material_system *system, t_material material)
{
	t_material* material_memory = malloc(sizeof(t_material));
	*material_memory = material;

	map_str_material_emplace(&system->materials, material.name, material_memory);
	return map_str_material_get_mut(&system->materials, material.name)->second;
}

t_shader *material_system_shader_add(t_material_system *system, t_shader shader)
{
	t_shader* shader_memory = malloc(sizeof(t_shader));
	*shader_memory = shader;
	map_str_shader_emplace(&system->shaders, shader.name, shader_memory);
	t_shader* ret = map_str_shader_get_mut(&system->shaders, shader.name)->second;
	return ret;
}