#ifndef COMPONENT_REGISTRY_GEN_H
#define COMPONENT_REGISTRY_GEN_H
#include "components.h"

#define i_tag i_crg_tag
#define i_key i_crg_type
#include "stc/vec.h"

#define c_JOIN0(a, b) a ## b
#define c_JOIN(a, b) c_JOIN0(a, b)
#define registerFuncName(name) 
#define VECTYPE c_JOIN(vec_, i_crg_tag)
#define INITFUNC c_JOIN(vec_, c_JOIN(i_crg_tag, _init))

#ifdef i_crg_header
void c_JOIN(component, c_JOIN(_register_type_, i_crg_tag))(t_component_system *system);
#endif

#ifndef i_crg_header
void c_JOIN(component, c_JOIN(_register_type_, i_crg_tag))(t_component_system *system) {
	VECTYPE *vec = malloc(sizeof(VECTYPE));
	*vec = INITFUNC();
	component_register(system, (t_component_group){ .name = i_crg_str_name, .size = sizeof(i_crg_type), .stc_vec_array = vec });
}
#endif

#undef c_JOIN0
#undef c_JOIN

#undef VECTYPE
#undef INITFUNC

#endif