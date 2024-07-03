#include "entity.h"

static t_entity *defaults[] =
{
   [ET_BASE] = &(t_entity){0}
};

static size_t sizes[] = 
{
    [ET_BASE] = sizeof(t_entity)
};

t_entity *entity_create(t_entitytype type)
{
    t_entity *entity = malloc(sizes[type]);
    memcpy(entity, defaults[type], sizes[type]);
}

t_entity *entity_destroy(t_entity *entity)
{
    free(entity);
}