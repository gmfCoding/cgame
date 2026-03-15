#include "cont/vec_vec3.h"
#include "vector3.h"
#include "core/procmesh.h"

void mesh_calculate_normals(t_mesh *mesh);

void pm_grid_create(t_pm_grid* grid, int x_size, int y_size, bool gpu)
{
    *grid = (t_pm_grid){.x_size = x_size, .y_size = y_size};
    mesh_create(&grid->mesh);

    size_t vertex_count = (x_size+1) * (y_size+1);
    // Generate
    vec_vec3_reserve(&grid->mesh.vertices, vertex_count);
    vec_vec2_reserve(&grid->mesh.uvs, (x_size + 1) * (y_size + 1));

    for (int i = 0, y = 0; y <= y_size; y++)
    {
        for (int x = 0; x <= x_size; x++, i++)
        {
            vec_vec3_push(&grid->mesh.vertices, v3new(x, y, 0));
            grid->mesh.uvs.data[i] = v2new((float)x / x_size, (float)y / y_size);
        }
    }
    grid->mesh.uvs._len = grid->mesh.uvs._cap;

    vec_GPUIndex_reserve(&grid->mesh.indices, x_size*y_size*6);
	for (int ti = 0, vi = 0, y = 0; y < y_size; y++, vi++) {
        for (int x = 0; x < x_size; x++, ti += 6, vi++) {


            grid->mesh.indices.data[ti] = vi;
            grid->mesh.indices.data[ti + 1] = vi + 1;
            grid->mesh.indices.data[ti + 2] = vi + x_size + 1;

            grid->mesh.indices.data[ti + 3] = vi + 1;
            grid->mesh.indices.data[ti + 4] = vi + x_size + 2;
            grid->mesh.indices.data[ti + 5] = vi + x_size + 1;
         
            
            // PREVIOUS
            /*
            grid->mesh.indices.data[ti] = vi;
            grid->mesh.indices.data[ti + 3] = grid->mesh.indices.data[ti + 2] = vi + 1;
            grid->mesh.indices.data[ti + 4] = grid->mesh.indices.data[ti + 1] = vi + x_size + 1;
            grid->mesh.indices.data[ti + 5] = vi + x_size + 2;
            */
        }
    }
    grid->mesh.indices._len = grid->mesh.indices._cap;
    mesh_calculate_normals(&grid->mesh);
    if (gpu)
        grid->gpu_mesh = gpu_mesh_add(&grid->mesh);

}


void mesh_calculate_normals(t_mesh *mesh)
{
    size_t vertex_count;
    size_t index_count;

    if (mesh == NULL)
        return;
    vertex_count = mesh->vertices._len;
    index_count = mesh->indices._len;

    vec_vec3_drop(&mesh->normals);
    mesh->normals = (vec_vec3){0};
    if (vertex_count == 0)
        return;

    vec_vec3_reserve(&mesh->normals, vertex_count);
    for (size_t i = 0; i < vertex_count; ++i)
        mesh->normals.data[i] = v3new(0, 0, 0);
    mesh->normals._len = vertex_count;

    for (size_t i = 0; i + 2 < index_count; i += 3)
    {
        GPUIndex i0 = mesh->indices.data[i];
        GPUIndex i1 = mesh->indices.data[i + 1];
        GPUIndex i2 = mesh->indices.data[i + 2];

        if (i0 >= vertex_count || i1 >= vertex_count || i2 >= vertex_count)
            continue;

        t_vec3 v0 = mesh->vertices.data[i0];
        t_vec3 v1 = mesh->vertices.data[i1];
        t_vec3 v2 = mesh->vertices.data[i2];
        t_vec3 edge1 = v3sub(v1, v0);
        t_vec3 edge2 = v3sub(v2, v0);
        t_vec3 face_normal = v3cross(edge1, edge2);
        face_normal = v3inv(face_normal);

        mesh->normals.data[i0] = v3add(mesh->normals.data[i0], face_normal);
        mesh->normals.data[i1] = v3add(mesh->normals.data[i1], face_normal);
        mesh->normals.data[i2] = v3add(mesh->normals.data[i2], face_normal);
    }

    for (size_t i = 0; i < vertex_count; ++i)
        mesh->normals.data[i] = v3norm(mesh->normals.data[i]);
    mesh->normals._len = mesh->normals._cap;
}