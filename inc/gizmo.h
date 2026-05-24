#ifndef GIZMO_H
#define GIZMO_H
#include "gpu/mesh.h"
#include <cglm/types.h>

enum e_gizmo_type
{
    GIZMO_LINE,
    GIZMO_CUBE,
    GIZMO_SPHERE
};

typedef struct s_gizmo
{
    enum e_gizmo_type type;
    bool persistent;
    vec3 colour;
    vec3 position;

    union {
        struct {
            t_gpu_line_list lines;
            vec3 end;
        } line;
        struct {
            t_gpu_mesh* u_mesh;
            vec3 scale;
        } cube;
        struct {
            t_gpu_mesh* u_mesh;
            float radius;
        } sphere;
    };
} t_gizmo;

typedef struct s_gismo_array
{
    t_gizmo* data;
    size_t _len;
    size_t _cap;
} t_gizmo_array;

typedef struct s_gizmo_register
{
    t_gizmo_array gizmo_frame;
    t_gizmo_array gizmo_persist;

} t_gizmo_register;

/// @brief Adds a gizmo to the render queue.
/// @param engine engine instance
/// @param gizmo gizmo to add
void gizmo_add(t_engine* engine, t_gizmo gizmo);

/// @brief Adds a line gizmo to the render queue.
/// @param engine engine instance
/// @param start start position of the line
/// @param end end oistion of the line
/// @param colour coilour of the line
/// @param persistent single frame or multiframe life time
void gizmo_line(t_engine* engine, vec3 start, vec3 end, vec3 colour, bool persistent);

/// @brief  Adds a cube gizmo to the render queue.
/// @param engine engine instance
/// @param position position of the cube
/// @param scale scale of the cube
/// @param colour colour of the cube
/// @param persistent single frame or multiframe life time
void gizmo_cube(t_engine* engine, vec3 position, vec3 scale, vec3 colour, bool persistent);

/// @brief Renders a line gizmo.
/// @param ctx render context
/// @param gizmo gizmo to render
void gizmo_line_renderer(t_render_ctx* ctx, t_gizmo* gizmo);

/// @brief Renders a cube gizmo.
/// @param ctx render context
/// @param gizmo gizmo to render
void gizmo_cube_render(t_render_ctx* ctx, t_gizmo* gizmo);

/// @brief Renders all gizmos in the engine's gizmo register.
/// @param engine engine instance
void render_gizmos(t_engine* engine);

#endif