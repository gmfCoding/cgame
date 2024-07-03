#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <cglm/quat.h>

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