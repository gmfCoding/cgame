#include "entity.h"
#include <cglm/affine.h>
#include <cglm/quat.h>
#include "entity/light.h"

static t_entity *defaults[] =
{
   [ET_BASE] = &(t_entity){.transform.scale={1, 1, 1}, 0},
   [ET_LIGHT] = (t_entity*)&(t_light){.base.transform.scale={1, 1, 1}, .light = {0}}
};

static size_t sizes[] = 
{
    [ET_BASE] = sizeof(t_entity),
    [ET_LIGHT] = sizeof(t_light),
};

void transform_get_mat4(t_transform *transform, mat4 value)
{
    glm_mat4_identity(value);
    glm_translate(value, transform->position);
    glm_quat_rotate(value, transform->rotation, value);
    glm_scale(value, transform->scale);
    return;
}

t_entity* entity_create_unowned(t_entitytype type, t_entity* entity)
{
    static uint32_t latest = 0;
    memcpy(entity, defaults[type], sizes[type]);
    entity->entity_id = latest++;
    return entity;
}

t_entity *entity_create(t_entitytype type)
{
    t_entity *entity = malloc(sizes[type]);
    return entity_create_unowned(type, entity);
}

void entity_destroy(t_entity *entity)
{
    free(entity);
}