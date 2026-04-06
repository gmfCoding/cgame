#ifndef ENT_LIGHT_H
#define ENT_LIGHT_H

#include "entity.h"
enum e_light_type
{
    LT_POINT,
    LT_DIRECTION
};
typedef struct s_light t_light;
struct s_light
{
    t_entity base;
    struct
    {
        enum e_light_type type;
        union
        {
            struct {
                float constant;
                float linear;
                float quadratic;

                uint8_t index;
            } point;
            struct {
                t_vec3 direction;
            } directional;
        };

        t_vec3 ambient;
        t_vec3 diffuse;
        t_vec3 specular;
    } light;
};
#endif