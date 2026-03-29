#include <inttypes.h>
#include <stdio.h>
#include "model.h"
#include "cont/vec_vec3.h"

void mesh_create(t_mesh *mesh)
{
	mesh->vertices = (vec_vec3){0};
	mesh->indices = (vec_GPUIndex){0};
	mesh->normals = (vec_vec3){0};
	mesh->uvs = (vec_vec2){0};
}

t_model *model_create_empty()
{
	t_model *model = malloc(sizeof(t_model));
	*model = (t_model){0};
	mesh_create(&model->mesh);

	model->normal_indices = (vec_GPUIndex){0};

	return model;
	// model->vertices = array_init(0, sizeof(t_vec3));
	// model->indices = array_init(0, sizeof(GPUIndex));
	// model->normals = array_init(0, sizeof(t_vec3));
	// model->normal_indices = array_init(0, sizeof(GPUIndex));
	// model->uvs = array_init(0, sizeof(t_vec2));
}

void model_explode(t_model *model)
{
	vec_vec3 temp_vertices;
	vec_GPUIndex temp_indices;

	int offset = -1;
	for (unsigned int i = 0; i < model->mesh.indices._len; i++)
	{
		if (model->mesh.indices.data[i] < 1)
		{
			offset = 0;
			break;
		}
	}

	for (unsigned int i = 0; i < model->mesh.indices._len; i += 3)
	{
		GPUIndex vertexIndex1 = model->mesh.indices.data[i + 0];
		GPUIndex vertexIndex2 = model->mesh.indices.data[i + 1];
		GPUIndex vertexIndex3 = model->mesh.indices.data[i + 2];

		t_vec3 vertex1 = model->mesh.vertices.data[vertexIndex1 + offset];
		t_vec3 vertex2 = model->mesh.vertices.data[vertexIndex2 + offset];
		t_vec3 vertex3 = model->mesh.vertices.data[vertexIndex3 + offset];

		vec_vec3_push_back(&temp_vertices, vertex1);
		vec_vec3_push_back(&temp_vertices, vertex2);
		vec_vec3_push_back(&temp_vertices, vertex3);

		vec_GPUIndex_push_back(&temp_indices, vertexIndex1);
		vec_GPUIndex_push_back(&temp_indices, vertexIndex2);
		vec_GPUIndex_push_back(&temp_indices, vertexIndex3);
	}

	vec_vec3_drop(&model->mesh.vertices);
	vec_GPUIndex_drop(&model->mesh.indices);

	model->mesh.vertices = temp_vertices;
	model->mesh.indices = temp_indices;
	model->flattened = true;
}

void model_flatten(t_model *model)
{
	vec_vec3 temp_vertices;
	vec_GPUIndex temp_indices;

	bool encountedSmall = false;
	for (unsigned int i = 0; i < model->mesh.indices._len; i++)
	{
		if (model->mesh.indices.data[i] < 1)
		{
			encountedSmall = true;
			break;
		}
	}

	for (unsigned int i = 0; i < model->mesh.indices._len; i++)
	{
		GPUIndex vertexIndex = model->mesh.indices.data[i];
		t_vec3 vertex;
		if (encountedSmall)
		{
			vertex = model->mesh.vertices.data[vertexIndex];
		}
		else
		{
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

typedef struct vertex
{
	GPUIndex i;
	GPUIndex t;
	GPUIndex n;
} vertex;

typedef struct face
{
	vertex v[3]
} face;

#define i_tag face
#define i_key face
#define i_header // Do not implement, only expose
#define i_implement
#include "stc/vec.h"

t_model *model_load(const char *file_path)
{
	t_model *model = model_create_empty();

	// vec_GPUIndex vertex_indices = {0}, uv_indices = {0}, normal_indices = {0};
	vec_face face_vertex_indices = {0};

	vec_vec3 temp_vertices = {0};
	vec_vec2 temp_uvs = {0};
	vec_vec3 temp_normals = {0};
	map_GPUIndex vertex_normal_map = map_GPUIndex_init();

	size_t face_count = 0;

	FILE *file = fopen(file_path, "r");
	if (file == NULL)
	{
		printf("Cannot open the file: %s!\n", file_path);
		return NULL;
	}
	bool smooth = false;
	while (1)
	{
		char lineHeader[128];

		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0)
		{
			static t_vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vec_vec3_push_back(&temp_vertices, vertex);
		}
		else if (strcmp(lineHeader, "s 1") == 0)
		{
			smooth = true;
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			t_vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			vec_vec3_push_back(&temp_normals, normal);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			t_vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			vec_vec2_push_back(&temp_uvs, uv);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			static GPUIndex vertexIndex[4], uvIndex[4], normalIndex[4];
			
			int matches = fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u", 
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
				 &vertexIndex[2], &uvIndex[2], &normalIndex[2],
				 &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
			vertexIndex[0] -= 1;
			vertexIndex[1] -= 1;
			vertexIndex[2] -= 1;
			normalIndex[0] -= 1;
			normalIndex[1] -= 1;
			normalIndex[2] -= 1;
			uvIndex[0] -= 1;
			uvIndex[2] -= 1;
			uvIndex[1] -= 1;
			if (matches != 9)
			{
				if (matches == 12)
				{
					printf("Found quad! This is not supported yet, try exporting as triangles\n");
				}
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return NULL;
			}
			else
			{
				face f = {.v = {{.i = vertexIndex[0], .t = uvIndex[0], .n = normalIndex[0]}, {.i = vertexIndex[1], .t = uvIndex[1], .n = normalIndex[1]}, {.i = vertexIndex[2], .t = uvIndex[2], .n = normalIndex[2]}}};
				vec_face_push_back(&face_vertex_indices, f);
				face_count++;
			}
		}
	}

	// Split vertices that share the same position but have different normals (or uvs)
	// Split vertices by unshared normals
	for (size_t i = 0; i < face_count; i++)
	{
		face *f1 = &face_vertex_indices.data[i];

		for (size_t v = 0; v < 3; v++)
		{
			for (size_t oi = 0; oi < face_count; oi++)
			{
				face *f2 = &face_vertex_indices.data[oi];

				for (size_t ov = 0; ov < 3; ov++)
				{
					if (f2->v[ov].i != f1->v[v].i)
						continue;
					if (f2->v[ov].n == f1->v[v].n)
						continue;
					// Found same vertex with different normal
					//printf("Found shared vertex with different normal! %zu %zu\n", i, oi);
					vec_vec3_push_back(&temp_vertices, temp_vertices.data[f2->v[ov].i]);
					f1->v[v].i = temp_vertices._len - 1;
					// vec_GPUIndex_push_back(&vertex_indices, temp_vertices._len);

					// vec_GPUIndex_push_back(&normal_indices, *ni);

					// vec_vec2_push_back(&temp_uvs, temp_uvs.data[uv_indices.data[s]]);
					// vec_GPUIndex_push_back(&uv_indices, temp_uvs._len);
				}
			}
		}
	}

	// for (size_t i = 0; i < face_count; i++)
	// {
	// 	for (size_t v = 0; v < 3; v++)
	// 	{
	// 		GPUIndex* vi = &vertex_indices.data[i*3+v];
	// 		GPUIndex* ni = &normal_indices.data[i*3+v];

	// 		// search for other uses of *vi
	// 		size_t max = vertex_indices._len;
	// 		for (size_t s = 0; s < max; s++)
	// 		{
	// 			if (vertex_indices.data[s] != *vi)
	// 				continue;
	// 			if (normal_indices.data[s] == *ni)
	// 				continue;
	// 			// Found same vertex with different normal
	// 			vec_vec3_push_back(&temp_vertices, temp_vertices.data[vertex_indices.data[s]]);
	// 			*vi = temp_vertices._len - 1;
	// 			//vec_GPUIndex_push_back(&vertex_indices, temp_vertices._len);

	// 			//vec_GPUIndex_push_back(&normal_indices, *ni);

	// 			//vec_vec2_push_back(&temp_uvs, temp_uvs.data[uv_indices.data[s]]);
	// 			//vec_GPUIndex_push_back(&uv_indices, temp_uvs._len);
	// 		}
	// 	}
	// }
	// delete unused vertices (that index isn't in vertex_indices) on each face
	for (size_t i = 0; i < temp_vertices._len; i++)
	{
		for (size_t f = 0; f < face_count; f++)
		{
			face *f1 = &face_vertex_indices.data[f];
			for (size_t v = 0; v < 3; v++)
			{
				if (f1->v[v].i == i)
				{
					goto used;
				}
			}
		}
		if (true)
		{
			for (size_t fi = 0; fi < face_count; fi++)
			{
				face *face = &face_vertex_indices.data[fi];
				for (size_t v = 0; v < 3; v++)
				{
					if (face->v[v].i > i)
						face->v[v].i--;
				}
			}
			printf("Removing vertex %zu\n", i);
			vec_vec3_erase_at(&temp_vertices, vec_vec3_advance(vec_vec3_begin(&temp_vertices), i));
			i--; // Adjust index after removal
		}
	used:
	}
	// Old working copy
	// // delete unused vertices (that index isn't in vertex_indices) on each face
	// for (size_t i = 0; i < temp_vertices._len; i++)
	// {

	// 	for (size_t s = 0; s < vertex_indices._len; s++)
	// 	{
	// 		if (vertex_indices.data[s] == i)
	// 		{
	// 			used = true;
	// 			break;
	// 		}
	// 	}
	// 	if (!used)
	// 	{
	// 		// Update indices above i to be one less, since we're removing this vertex
	// 		for (size_t s = 0; s < vertex_indices._len; s++)
	// 		{
	// 			if (vertex_indices.data[s] > i)
	// 				vertex_indices.data[s]--;
	// 		}
	// 		vec_vec3_iter test = {0};

	// 		vec_vec3_erase_at(&temp_vertices, vec_vec3_advance(vec_vec3_begin(&temp_vertices), i));
	// 		i--; // Adjust index after removal
	// 	}
	// }

	// Old working copy
	// // delete unused vertices (that index isn't in vertex_indices) on each face
	// for (size_t i = 0; i < temp_vertices._len; i++)
	// {

	// 	for (size_t s = 0; s < vertex_indices._len; s++)
	// 	{
	// 		if (vertex_indices.data[s] == i)
	// 		{
	// 			used = true;
	// 			break;
	// 		}
	// 	}
	// 	if (!used)
	// 	{
	// 		// Update indices above i to be one less, since we're removing this vertex
	// 		for (size_t s = 0; s < vertex_indices._len; s++)
	// 		{
	// 			if (vertex_indices.data[s] > i)
	// 				vertex_indices.data[s]--;
	// 		}
	// 		vec_vec3_iter test = {0};

	// 		vec_vec3_erase_at(&temp_vertices, vec_vec3_advance(vec_vec3_begin(&temp_vertices), i));
	// 		i--; // Adjust index after removal
	// 	}
	// }

	// for (intptr_t i = 0; i < vertex_indices._len; i++)
	// {
	// 	vec_GPUIndex_push_back(&model->mesh.indices, vertex_indices.data[i]);
	// }
	vec_GPUIndex vertex_indices = {0};

	model->mesh.normals = (struct vec_vec3){0};
	model->has_normals = true;

	vec_vec3_reserve(&model->mesh.normals, temp_vertices._len);
	vec_vec2_reserve(&model->mesh.uvs, temp_vertices._len);
	// layout normals and vertices in the same order as the indices
	for (size_t i = 0; i < face_count; i++)
	{
		face *f = &face_vertex_indices.data[i];
		for (size_t v = 0; v < 3; v++)
		{
			vec_GPUIndex_push_back(&vertex_indices, f->v[v].i);
			model->mesh.normals.data[f->v[v].i] = temp_normals.data[f->v[v].n];
			model->mesh.uvs.data[f->v[v].i] = temp_uvs.data[f->v[v].t];
			//vec_vec3_insert_at(&model->mesh.normals, vec_vec3_advance(vec_vec3_begin(&model->mesh.normals), f->v[v].i), temp_normals.data[f->v[v].n]);
		}
	}
	model->mesh.normals._len = temp_vertices._len;
	model->mesh.uvs._len = temp_vertices._len;

	model->mesh.vertices = temp_vertices;
	model->mesh.indices = vertex_indices;

	// for (int i = 0; i < uv_indices._len; i++) {
	// 	model->has_uv = true;
	// 	vec_vec2_push(&model->mesh.uvs, temp_uvs.data[uv_indices.data[i]]);
	// }

	// if (smooth)
	// {
	// 	model_flatten(model);
	// }
	// // else
	// // {
	// // 	model_explode(model);
	// // }

	// for (intptr_t i = 0; i < normal_indices._len; i++)
	// {
	// 	vec_vec3_push(&model->mesh.normals, temp_normals.data[normal_indices.data[i] - 1]);
	// }

	// if (model->has_normals)
	// {
	// 	for (intptr_t i = 0; i < model->mesh.vertices._len; i++)
	// 	{
	// 		if (map_GPUIndex_contains(&vertex_normal_map, i + 1) == false)
	// 			break;
	// 		GPUIndex index = map_GPUIndex_get(&vertex_normal_map, i + 1)->second;
	// 		// printf("%li/%d: %d,%d,%d\n", i, index, (int)temp_normals.data[index - 1].x, (int)temp_normals.data[index - 1].y, (int)temp_normals.data[index - 1].z);
	// 		vec_vec3_push_back(&model->mesh.normals, temp_normals.data[index - 1]);
	// 	}
	// }
	return model;
}
