#include <inttypes.h>
#include <stdio.h>
#include "model.h"

t_model *model_create_empty()
{
    t_model *model = malloc(sizeof(t_model));
    *model = (t_model){0};

    model->mesh.vertices = (vec_vec3){0};
    model->mesh.indices = (vec_GPUIndex){0};
    model->mesh.normals = (vec_vec3){0};
    model->normal_indices = (vec_GPUIndex){0};
    model->mesh.uvs = (vec_vec2){0};

    return model;
    // model->vertices = array_init(0, sizeof(t_vec3));
    // model->indices = array_init(0, sizeof(GPUIndex));
    // model->normals = array_init(0, sizeof(t_vec3));
    // model->normal_indices = array_init(0, sizeof(GPUIndex));
    // model->uvs = array_init(0, sizeof(t_vec2));
}

void model_flatten(t_model *model)
{
    vec_vec3 temp_vertices;
    vec_GPUIndex temp_indices;

    bool encountedSmall = false;
    for (unsigned int i = 0; i < model->mesh.indices._len; i++) {
        if (model->mesh.indices.data[i] < 1)
        {
            encountedSmall = true;
            break;
        }
    }

    for (unsigned int i = 0; i < model->mesh.indices._len; i++) {
        GPUIndex vertexIndex = model->mesh.indices.data[i];
        t_vec3 vertex;
        if (encountedSmall) {
            vertex = model->mesh.vertices.data[vertexIndex];
        }
        else {
            vertex = model->mesh.vertices.data[vertexIndex - 1];
        }

        vec_vec3_push_back(&temp_vertices, vertex);
        vec_GPUIndex_push_back(&temp_indices, vertexIndex);
    }

    vec_vec3_drop(&model->mesh.vertices);
    vec_GPUIndex_drop(&model->mesh.indices);

    model->mesh.vertices = temp_vertices;
	model->mesh.indices = temp_indices;
    model->flattened = true;
}

t_model *model_load(const char* file_path)
{
    t_model *model = model_create_empty();

    vec_GPUIndex vertex_indices = {0}, uv_indices = {0}, normal_indices = {0};

    vec_vec3 temp_vertices = {0};
    // vec_vec2 temp_uvs = {0};
    vec_vec3 temp_normals = {0};
    smap_GPUIndex vertex_normal_map = smap_GPUIndex_init();

	FILE* file = fopen(file_path, "r");
	if (file == NULL) {
		printf("Cannot open the file: %s!\n", file_path);
		return NULL;
	}

	while (1) {
		char lineHeader[128];

		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			static t_vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vec_vec3_push_back(&temp_vertices, vertex);
        }
		else if (strcmp(lineHeader, "vn") == 0) {
			t_vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            vec_vec3_push_back(&temp_normals, normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			static GPUIndex vertexIndex[3], uvIndex[3], normalIndex[3];
						
			int matches = fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return NULL;
			}
			else
			{
                smap_GPUIndex_insert_or_assign(&vertex_normal_map, vertexIndex[0], normalIndex[0]);
                smap_GPUIndex_insert_or_assign(&vertex_normal_map, vertexIndex[1], normalIndex[1]);
                smap_GPUIndex_insert_or_assign(&vertex_normal_map, vertexIndex[2], normalIndex[2]);

				vec_GPUIndex_push_back(&vertex_indices, vertexIndex[0]);
				vec_GPUIndex_push_back(&vertex_indices, vertexIndex[1]);
				vec_GPUIndex_push_back(&vertex_indices, vertexIndex[2]);
				vec_GPUIndex_push_back(&uv_indices, uvIndex[0]);
				vec_GPUIndex_push_back(&uv_indices, uvIndex[1]);
				vec_GPUIndex_push_back(&uv_indices, uvIndex[2]);
				vec_GPUIndex_push_back(&normal_indices, normalIndex[0]);
				vec_GPUIndex_push_back(&normal_indices, normalIndex[1]);
				vec_GPUIndex_push_back(&normal_indices, normalIndex[2]);
			}

		}
	}
    
	for (intptr_t i = 0; i < vertex_indices._len; i++)
	{
		vertex_indices.data[i] -= 1;
	}
	model->mesh.vertices = temp_vertices;
	model->mesh.indices = vertex_indices;
	if (normal_indices._len > 0)
		model->has_normals = true;
	if (temp_normals._len == model->mesh.indices._len)
	{
        model_flatten(model);
		for (intptr_t i = 0; i < normal_indices._len; i++)
		{
            vec_vec3_push(&model->mesh.normals, temp_normals.data[normal_indices.data[i] - 1]);
		}
	}
	if (model->has_normals)
	{
		for (intptr_t i = 0; i < model->mesh.vertices._len; i++)
		{
            if (smap_GPUIndex_contains(&vertex_normal_map, i + 1) == false)
                break;
            GPUIndex index = smap_GPUIndex_get(&vertex_normal_map, i + 1)->second;
            vec_vec3_push_back(&model->mesh.normals, temp_normals.data[index - 1]);
		}
	}
	return model;
}
