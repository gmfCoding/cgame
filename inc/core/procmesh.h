#ifndef PROC_MESH_H
#include "gpu/mesh.h"
#include "model.h"

typedef struct s_pm_grid
{
    int x_size;
    int y_size;

    t_mesh mesh;
    t_gpu_mesh *gpu_mesh;
}  t_pm_grid;

void pm_grid_create(t_pm_grid* grid, int x_size, int y_size, bool gpu);
#endif