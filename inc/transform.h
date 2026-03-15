#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <cglm/quat.h>

#define TTRANSFORM_IDENTITY (t_transform){.position={0,0,0}, .rotation=GLM_QUAT_IDENTITY_INIT, .scale={1,1,1}}

typedef struct s_transform
{
    vec3 position;
    versor rotation;
    vec3 scale;

    /* data */
} t_transform;


#include <cglm/mat4.h>
void transform_get_mat4(t_transform *transform, mat4 out);

#endif