#ifndef ENTITY_COMPONENTS
#define ENTITY_COMPONENTS
#include "cont/components.h"

//g_component_transform_id = 

t_component_type component_register(t_component_system *system, t_component_group group)
{
	system->next_id++;
	system->count++;

	group.type = system->next_id;
	map_str_component_type_emplace(&system->registered_components, group.name, system->next_id);
	return (system->next_id);
}

#include <cglm/mat4.h>	

typedef struct s_mat4_wrap
{
	mat4 value;
} t_mat4_wrap;


#define i_crg_str_name "transform"
#define i_crg_tag ComponentTransform
#define i_crg_type t_mat4_wrap
#include "cont/component_registery_gen.h"

void component_setup_base()
{
	t_component_system system;
	
	component_register_type_ComponentTransform(&system);
	// vec_ComponentTransform *vec = malloc(sizeof(vec_ComponentTransform));
	// *vec = vec_ComponentTransform_init();
	// component_register(&system, (t_component_group){ .name = "transform", .size = sizeof(mat4), .stc_vec_array = vec });

	// vec_ComponentTransform *vec = malloc(sizeof(vec_ComponentTransform));
	// component_register(&system, (t_component_group){ .name = "transform", .size = sizeof(mat4), .stc_vec_array = vec });
	// *vec = vec_ComponentTransform_init();
}
#endif