#ifndef LIGHTING_H
#define LIGHTING_H

#include "vector3.h"
#include "material.h"
typedef struct s_light t_light;
typedef enum e_light_type t_light_type;

void material_light_update(t_material* material, t_light* light);
void material_prop_add_new_light(t_light_type type, uint8_t lightIndex, t_material* material);

#endif