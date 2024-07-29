#ifndef ENTITY_COMPONENTS
#define ENTITY_COMPONENTS

typedef unsigned int t_component_type;
typedef unsigned int t_entity_id;

typedef struct s_component_group
{
	t_component_type	type;
	char				*name;
	size_t				size;
	void				*stc_vec_array;
} t_component_group;

typedef struct s_component_system
{
	size_t count;
	t_component_type next_id;
	map_component_types registered_components;
	map_component_groups groups;
} t_component_system;

#define i_type map_component_groups
#define i_key t_component_type
#define i_val t_component_group
#define i_header
#include "stc/hmap.h"

#define i_type map_component_types
#define i_key_str 
#define i_val t_component_type
#define i_header
#include "stc/hmap.h"

//g_component_transform_id = 

t_component_type component_register(t_component_system *system, t_component_group group)
{
	system->next_id++;
	system->count++;

	group.type = system->next_id;
	map_component_types_emplace(&system->registered_components, group.name, system->next_id);
	return (system->next_id);
}

#include <cglm/mat4.h>

#define i_tag ComponentTransform
#define i_key mat4
#define i_header    // Do not implement, only expose 
#include "stc/vec.h"

void test()
{
	vec_ComponentTransform *vec = malloc(sizeof(vec_ComponentTransform));
	*vec = vec_ComponentTransform_init();
	component_register(&system, (t_component_group){ .name = "transform", .size = sizeof(mat4), .stc_vec_array = vec });
}
#endif