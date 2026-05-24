#include "cont/vec_vec3.h"
#include "vector3.h"
#include "core/procmesh.h"

void mesh_calculate_normals(t_mesh *mesh);

typedef struct coro_t_		coro_t;

extern coro_t* coro;
void coro_yield(coro_t *, int);

void pm_rounded_cube_create(t_pm_rounded_cube* proc, int x_size, int y_size, int z_size, float roundness, bool gpu)
{
    *proc = (t_pm_rounded_cube){.x_size = x_size, .y_size = y_size, .z_size = z_size, .roundness = roundness};
    mesh_create(&proc->mesh);

    //size_t vertex_count = (x_size+1) * (y_size+1);
    int cornerVertices = 8;
    int edgeVertices = (x_size + y_size + z_size - 3) * 4;
    int faceVertices = (
        (x_size - 1) * (y_size - 1) +
        (x_size - 1) * (z_size - 1) +
        (y_size - 1) * (z_size - 1)) * 2;

    // Generate
    vec_vec3_reserve(&proc->mesh.vertices, cornerVertices + edgeVertices + faceVertices);
    vec_vec2_reserve(&proc->mesh.uvs, (x_size + 1) * (y_size + 1) * (z_size + 1));

    coro_yield(coro, 0);

    int v = 0;
    for (int y = 0; y <= y_size; y++) {
        for (int x = 0; x <= x_size; x++) {
            proc->mesh.vertices.data[v++] = v3new(x, y, 0);
            coro_yield(coro, v);
        }
        for (int z = 1; z <= z_size; z++) {
            proc->mesh.vertices.data[v++] = v3new(x_size, y, z);
            coro_yield(coro, v);
        }
        for (int x = x_size - 1; x >= 0; x--) {
            proc->mesh.vertices.data[v++] = v3new(x, y, z_size);
            coro_yield(coro, v);
        }
        for (int z = z_size - 1; z > 0; z--) {
            proc->mesh.vertices.data[v++] = v3new(0, y, z);
            coro_yield(coro, v);
        }
    }

    // Cap the top face
    for (int z = 1; z < z_size; z++) {
        for (int x = 1; x < x_size; x++) {
            proc->mesh.vertices.data[v++] = v3new(x, y_size, z);
            coro_yield(coro, v);    
        }
    }

    // Cap the bottom face
    for (int z = 1; z < z_size; z++) {
        for (int x = 1; x < x_size; x++) {
            proc->mesh.vertices.data[v++] = v3new(x, 0, z);
            coro_yield(coro, v);
        }
    }

    proc->mesh.vertices._len = v;

    // for (int i = 0, y = 0; y <= y_size; y++)
    // {
    //     for (int x = 0; x <= x_size; x++, i++)
    //     {
    //         vec_vec3_push(&proc->mesh.vertices, v3new(x, y, 0));
    //         proc->mesh.uvs.data[i] = v2new((float)x / x_size, (float)y / y_size);
    //     }
    // }
    // proc->mesh.uvs._len = proc->mesh.uvs._cap;

    // vec_GPUIndex_reserve(&proc->mesh.indices, x_size*y_size*6);
	// for (int ti = 0, vi = 0, y = 0; y < y_size; y++, vi++) {
    //     for (int x = 0; x < x_size; x++, ti += 6, vi++) {


    //         proc->mesh.indices.data[ti] = vi;
    //         proc->mesh.indices.data[ti + 1] = vi + 1;
    //         proc->mesh.indices.data[ti + 2] = vi + x_size + 1;

    //         proc->mesh.indices.data[ti + 3] = vi + 1;
    //         proc->mesh.indices.data[ti + 4] = vi + x_size + 2;
    //         proc->mesh.indices.data[ti + 5] = vi + x_size + 1;
         
            
    //         // PREVIOUS
    //         /*
    //         proc->mesh.indices.data[ti] = vi;
    //         proc->mesh.indices.data[ti + 3] = proc->mesh.indices.data[ti + 2] = vi + 1;
    //         proc->mesh.indices.data[ti + 4] = proc->mesh.indices.data[ti + 1] = vi + x_size + 1;
    //         proc->mesh.indices.data[ti + 5] = vi + x_size + 2;
    //         */
    //     }
    // }
    // proc->mesh.indices._len = proc->mesh.indices._cap;
    // mesh_calculate_normals(&proc->mesh);
    if (gpu)
        proc->gpu_mesh = gpu_mesh_add(&proc->mesh, .points_only = true);
}