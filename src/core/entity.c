#include "entity.h"
#include <cglm/affine.h>
#include <cglm/quat.h>

static t_entity *defaults[] =
{
   [ET_BASE] = &(t_entity){0}
};

static size_t sizes[] = 
{
    [ET_BASE] = sizeof(t_entity)
};

void transform_get_mat4(t_transform *transform, mat4 value)
{
	mat4 quat;

	glm_translate(value, transform->position);
	glm_quat_mat4(transform->rotation, quat);
	glm_mat4_mul(value, quat, value);
    glm_scale(value, transform->scale);
    return;
}

t_entity *entity_create(t_entitytype type)
{
    t_entity *entity = malloc(sizes[type]);
    memcpy(entity, defaults[type], sizes[type]);
	return entity;
}

void entity_destroy(t_entity *entity)
{
    free(entity);
}