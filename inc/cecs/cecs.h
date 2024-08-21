#pragma once

extern const int c_ECS_MAX_TYPES = 32768;

typedef int EcsType;
typedef int EcsArchetype;
typedef int EcsEnt;

typedef struct EcsSystem
{

} EcsSystem;


#define i_type map_str_EcsArchetype
#define i_key_str
#define i_val EcsArchetype
#define i_header    // Do not implement, only expose 
#include "stc/hmap.h"

#define i_tag EcsArchetype
#define i_key EcsArchetype
#define i_header    // Do not implement, only expose 
#include "stc/vec.h"

typedef struct EcsContainer
{
	EcsSystem systems[c_ECS_MAX_TYPES];

	map_str_EcsArchetype archetypes;

	EcsType type_count;

} EcsC;

typedef struct EcsArchetypeOptions
{
	EcsArchetype type;
} EcsArchetypeOptions;

typedef void (*FnEcsSystem)(void *data, int length, void *context);

EcsType ecs_structure_create(EcsC *ecsc, int size);

// Creates an archetype
EcsArchetype ecs_archetype_create(EcsC *ecsc, char *name, int size);

EcsArchetype ecs_archetype_get(EcsC *ecsc, char *name);

// Adds a component to an archetype
void ecs_archetype_emplace(EcsC *ecsc, EcsType type, void *defaults);

// Create an entity from an archetype
EcsEnt ecs_entity_create(EcsC *ecsc, EcsArchetypeOptions *options);

void *ecs_component_get(EcsC* ecsc, EcsEnt ent, EcsType type);

void ecs_system_create(EcsC *ecsc, EcsType type, EcsSystem system, void *context);

void ecs_autoupdate();