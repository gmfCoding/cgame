#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <stddef.h>
#include <stc/cstr.h>

typedef unsigned int t_component_type;
typedef unsigned int t_entity_id;

typedef struct s_component_group
{
	t_component_type	type;
	char				*name;
	size_t				size;
	void				*stc_vec_array;
} t_component_group;

#define i_type map_component_groups
#define i_key t_component_type
#define i_val t_component_group
//#define i_header    // Do not implement, only expose 
#include "stc/hmap.h"

#define i_type map_str_component_type
#define i_key_str
#define i_val t_component_type
//#define i_header    // Do not implement, only expose 
#include "stc/hmap.h"

typedef struct s_component_system
{
	size_t count;
	t_component_type next_id;
	map_str_component_type registered_components;
	map_component_groups groups;
} t_component_system;

#endif