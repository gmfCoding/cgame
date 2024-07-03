#ifndef MATERIAL_PROP_H
#define MATERIAL_PROP_H

#include "load_glad.h"

#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <cglm/ivec2.h>
#include <cglm/ivec3.h>
#include <cglm/ivec4.h>
#include <cglm/mat4.h>

typedef enum e_mat_prop_loc_status
{
	MPLS_UNLOADED,
	MPLS_LOADED,
	MPLS_NONEXIST,
} t_mat_prop_loc_status;

#define MPT_DEFAULT (t_mat_prop_value){0}

typedef union s_mat_prop_value
{
    GLboolean b;

    GLint i1;
    ivec2 i2;
    ivec3 i3;
    ivec4 i4;

    GLuint u1;
    GLuint u2[2];
    GLuint u3[3];
    GLuint u4[4];
    
    GLfloat f1;
    vec2 f2;
    vec3 f3;
    vec4 f4;

    mat4 mat;
} t_mat_prop_value;

typedef enum e_mat_prop_type
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

typedef struct s_mat_prop
{
    const char* name;
	t_mat_prop_value value;
	t_mat_prop_type type;
	GLint location;
	t_mat_prop_loc_status location_status;
    bool dirty;
} t_mat_prop;

#endif