#ifndef MATERIAL_H
#define MATERIAL_H
#include "material_prop.h"

typedef struct s_material_system t_material_system;
typedef struct s_shader t_shader;
typedef struct s_material t_material;

#include "stc/cstr.h"
#include "cont/vec_mat_prop.h"
#include "cont/map_mat_prop.h"
#include "cont/set_str.h"

typedef struct s_material
{
    const char* name;
	t_shader* shader;
	
    set_str prop_names;
    map_mat_prop properties;

    bool dirty;
    bool instanced;
} t_material;

#define GLCase(type, rtype, func) case type: \
			func(prop->location, prop->value.rtype); \
			break;

#define GLCaseV(TYPE, rtype, FUNC) case TYPE: \
			FUNC(prop->location, 1, &prop->value.rtype[0]); \
			break


void mat_prop_apply(t_material *mat, t_mat_prop *prop);

t_mat_prop *material_prop_get(t_material *mat, const char* name);
            
/// @brief Marks a material property as dirty
/// @param material 
/// @param name 
void material_prop_update(t_material *material, t_mat_prop *prop);

void material_apply(t_material *mat);

/// @brief Adds a new property to the given material
/// @param mat
/// @param prop (stack safe)
void material_prop_add(t_material *mat, t_mat_prop * prop);

void material_prop_add_new(t_material *mat, const char*name, t_mat_prop_type type, t_mat_prop_value value);

/// @brief Adds a range of new properties to the given material
/// @param mat
/// @param properties
void material_prop_add_range(t_material *mat, vec_mat_prop properties);
#endif