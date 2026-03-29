#ifndef MATERIAL_SYSTEM_H
# define MATERIAL_SYSTEM_H

#include "cont/map_str_material.h"
#include "cont/map_str_shader.h"

typedef struct s_material_system
{
	map_str_material materials;
	map_str_shader shaders;

} t_material_system;

/// @brief Initializes the material system, must be called before use.
/// @param system(out) - the material system to initialize
void material_system_init(t_material_system *system);

/// @brief Duplicates a material within the material system.
/// @param system(ref) - the material system to use
/// @param material(ref) - the material to duplicate
/// @param new_name(ref) - the name for the new material
t_material *material_system_mat_dup(t_material_system *system, t_material* material, const char* new_name);


/// @brief Retrieves a material from the material system by name.
/// @param system(ref) - the material system to use
/// @param name(ref) - the name of the material to find
t_material *material_system_mat_get(t_material_system *system, const char* name);

/// @brief Retrieves a shader from the material system by name.
/// @param system(ref) - the material system to use
/// @param name(ref) - the name of the shader to get
t_shader *material_system_shader_get(t_material_system *system, const char* name);

/// @brief Registers a new material within the material system.
/// @param system(ref) - the material system to use
/// @param material(copied) - the material to register
t_material *material_system_mat_create(t_material_system *system, t_material material);

/// @brief Registers a new shader to the material system.
/// @param system(ref) - the material system to use
/// @param shader(copied) - the shader to register
t_shader *material_system_shader_add(t_material_system *system, t_shader shader);

#endif