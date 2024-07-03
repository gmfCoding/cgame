#ifndef MATERIAL_H
#define MATERIAL_H
#include "load_glad.h"

#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <cglm/ivec2.h>
#include <cglm/ivec3.h>
#include <cglm/ivec4.h>
#include <cglm/mat4.h>

typedef union
{
    GLboolean b;

    GLint i1;
    ivec2 i2;
    ivec3 i3;
    ivec4 i4;

    GLuint u1[1];
    GLuint u2[2];
    GLuint u3[3];
    GLuint u4[4];
    
    GLfloat f1;
    vec2 f2;
    vec3 f3;
    vec4 f4;

    mat4 mat;
} t_mat_prop_value;

typedef enum
{
    MPT_BOOL,
    MPT_INT1,
    MPT_INT2,
    MPT_INT3,
    MPT_INT4,
    MPT_UINT1,
    MPT_UINT2,
    MPT_UINT3,
    MPT_UINT4,
    MPT_FLOAT1,
    MPT_FLOAT2,
    MPT_FLOAT3,
    MPT_FLOAT4,
    MPT_MAT4
} t_mat_prop_type;

typedef struct
{
    const char* name;
	t_mat_prop_value value;
	t_mat_prop_type type;
	GLint location;
    bool dirty;
} t_mat_prop;


#define i_TYPE vec_mat_prop, t_mat_prop
#include <stc/vec.h>

#include "stc/cstr.h"

#define i_TYPE map_mat_prop, i_key_str, t_mat_prop

#include "stc/hmap.h"

#define i_TYPE set_str, i_key_str
#include <stc/hset.h>

typedef struct
{
    const char* name;
    GLuint  program;

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


void mat_prop_apply(t_mat_prop *prop);

t_mat_prop *material_prop_get(const t_material *mat, const char* name);
            
/// @brief Marks a material property as dirty
/// @param material 
/// @param name 
void material_prop_update(t_material *material, t_mat_prop *prop);

void material_apply(t_material *mat);

/// @brief Adds a new property to the given material
/// @param mat
/// @param prop (stack safe)
void material_prop_add(t_material *mat, t_mat_prop *const prop);

/// @brief Adds a range of new properties to the given material
/// @param mat
/// @param properties
void material_prop_add_range(t_material *mat, vec_mat_prop properties);
#endif