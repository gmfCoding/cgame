#include "lighting.h"
#include "material.h"
#include "cglm/vec3.h"
#include "util.h"
#include "logging.h"
#include "entity/light.h"
#include <string.h>

static void apply_light_prop_vec3(t_material* material, vec3 value, char* prop_name)
{
    t_mat_prop* prop = material_prop_get(material, prop_name);
    glm_vec3_copy(value, prop->value.f3);
    material_prop_update(material, prop);
}

static void apply_light_point_prop_sized(t_material* material, uint8_t lightIndex, void* value, size_t size, char* prop_name)
{
    static char lightUniformElement[256] = "\0";
    lightUniformElement[0] = '\0';
    snprintf(lightUniformElement, sizeof(lightUniformElement) - 1, "pointLights[%u].%s", lightIndex, prop_name);
    t_mat_prop* prop = material_prop_get(material, lightUniformElement);
    memcpy(&prop->value, value, size);
    material_prop_update(material, prop);
}

static void apply_light_point_prop_sized_new(t_material* material, uint8_t lightIndex, char* prop_name, t_mat_prop_type type, t_mat_prop_value default_value)
{
    static char lightUniformElement[256] = "\0";
    lightUniformElement[0] = '\0';
    snprintf(lightUniformElement, sizeof(lightUniformElement) - 1, "pointLights[%u].%s", lightIndex, prop_name);
    material_prop_add_new(material, strdup(lightUniformElement), type, default_value);
}

void material_light_update(t_material* material, t_light* light)
{
    if (light->light.type == LT_DIRECTION)
    {
        apply_light_prop_vec3(material, light->light.directional.direction.v,  "directional.direction");
        apply_light_prop_vec3(material, light->light.ambient.v,                "directional.ambient");
        apply_light_prop_vec3(material, light->light.diffuse.v,                "directional.diffuse");
        apply_light_prop_vec3(material, light->light.specular.v,               "directional.specular");
    }
    else if (light->light.type == LT_POINT)
    {
        if (light->light.point.index >= material->max_lights)
        {
            te_logf(LOG_LEVEL_WARNING, "lighting", "Warning attempting to update out of bounds light index on material");
            return;
        }
        apply_light_point_prop_sized(material, light->light.point.index, &light->base.transform.position,  sizeof(light->base.transform.position), "position");
        apply_light_point_prop_sized(material, light->light.point.index, &light->light.point.constant,     sizeof(light->light.point.constant),    "constant");
        apply_light_point_prop_sized(material, light->light.point.index, &light->light.point.linear,       sizeof(light->light.point.linear),      "linear");
        apply_light_point_prop_sized(material, light->light.point.index, &light->light.point.quadratic,    sizeof(light->light.point.quadratic),   "quadratic");

        apply_light_point_prop_sized(material, light->light.point.index, &light->light.ambient,            sizeof(light->light.ambient),           "ambient");
        apply_light_point_prop_sized(material, light->light.point.index, &light->light.diffuse,            sizeof(light->light.diffuse),           "diffuse");
        apply_light_point_prop_sized(material, light->light.point.index, &light->light.specular,           sizeof(light->light.specular),          "specular");
    }   
}

void material_prop_add_new_light(t_light_type type, uint8_t lightIndex, t_material* material)
{
    if (type == LT_DIRECTION)
    {
        material_prop_add_new(material, "directional.direction", MPT_FLOAT3, (t_mat_prop_value){.f3={-0.2f, -1.0f, -0.3f}});
        material_prop_add_new(material, "directional.ambient",   MPT_FLOAT3, (t_mat_prop_value){.f3={0.2f, 0.2f, 0.2f}});
        material_prop_add_new(material, "directional.diffuse",   MPT_FLOAT3, (t_mat_prop_value){.f3={0.5f, 0.5f, 0.5f}});
        material_prop_add_new(material, "directional.specular",  MPT_FLOAT3, (t_mat_prop_value){.f3={1.0f, 1.0f, 1.0f}});
    }
    else if (type == LT_POINT)
    {
        if (lightIndex >= material->max_lights)
        {
            te_logf(LOG_LEVEL_WARNING, "lighting", "Warning attempting to add new light element which is out of bounds on material");
            return;
        }
        apply_light_point_prop_sized_new(material, lightIndex, "position",  MPT_FLOAT3, (t_mat_prop_value){.f3={0.0f, 0.0f, 0.0f}});
        apply_light_point_prop_sized_new(material, lightIndex, "constant",  MPT_FLOAT1, (t_mat_prop_value){.f1=1.0});
        apply_light_point_prop_sized_new(material, lightIndex, "linear",    MPT_FLOAT1, (t_mat_prop_value){.f1=1.0});
        apply_light_point_prop_sized_new(material, lightIndex, "quadratic", MPT_FLOAT1, (t_mat_prop_value){.f1=1.0});
        
        apply_light_point_prop_sized_new(material, lightIndex, "ambient",   MPT_FLOAT3, (t_mat_prop_value){.f3={0.0f, 0.0f, 0.0f}});
        apply_light_point_prop_sized_new(material, lightIndex, "diffuse",   MPT_FLOAT3, (t_mat_prop_value){.f3={0.0f, 0.0f, 0.0f}});
        apply_light_point_prop_sized_new(material, lightIndex, "specular",  MPT_FLOAT3, (t_mat_prop_value){.f3={0.0f, 0.0f, 0.0f}});
    }
}