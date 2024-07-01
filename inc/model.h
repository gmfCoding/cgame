/*
 * @Author: christopher.lovell
 * @Date: 2024-07-01 22:44:16
 * @Last Modified by: christopher.lovell
 * @Last Modified time: 2024-07-01 23:11:27
 */
#ifndef MODEL_H
#define MODEL_H

#include "vector3.h"
#include "vector2.h"
#include "cont/smap_gpui_gpui.h"
#include "cont/vec_vec2.h"
#include "cont/vec_vec3.h"
#include "cont/vec_gpuindex.h"

typedef struct
{
    /// @brief array containing t_vec3
    vec_vec3 vertices;

    /// @brief array containing GPUIndex
    vec_GPUIndex indices;

    /// @brief array containing t_vec3
    vec_vec3 normals;

    /// @brief array containing t_vec2
    vec_vec2 uvs;
} t_mesh;

typedef struct
{
    t_mesh mesh;

    /// @brief array containing GPUIndex
    vec_GPUIndex normal_indices;

    bool flattened;
    bool has_uv;
    bool has_normals;
} t_model;

void model_flatten(t_model *model);
t_model *model_create_empty();
t_model *model_load(const char* file_path);

#endif