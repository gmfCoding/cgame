#include "glad/glad.h"
#include "gldebug.h"
#include "renderer.h"
#include "engine.h"
#include "named_register.h"
#include "util.h"

#include "gizmo.h"


static void vec_gizmo_array_push(t_gizmo_array* arr, t_gizmo gizmo)
{
    if (arr->_len >= arr->_cap)
    {
        size_t new_cap = arr->_cap == 0 ? 4 : arr->_cap * 2;
        arr->data = realloc(arr->data, new_cap * sizeof(t_gizmo));
        arr->_cap = new_cap;
    }
    arr->data[arr->_len++] = gizmo;
}

void gizmo_add(t_engine* engine, t_gizmo gizmo)
{
    t_gizmo_register* gizmos = *named_register_get(engine, "gizmo_register");
    if (gizmo.persistent)
    {
        // setup opengl stuff for the gizmo here so it doesn't have to be done every frame
        GLCall(glGenVertexArrays(1, &gizmo.line.lines.m_vao));
        GLCall(glGenBuffers(1, &gizmo.line.lines.m_vbo));
        GLCall(glBindVertexArray(gizmo.line.lines.m_vao));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, gizmo.line.lines.m_vbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, NULL, GL_STATIC_DRAW));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

        vec_gizmo_array_push(&gizmos->gizmo_persist, gizmo);
    }
    else
        vec_gizmo_array_push(&gizmos->gizmo_frame, gizmo);

}

void gizmo_line(t_engine* engine, vec3 start, vec3 end, vec3 colour, bool persistent)
{
    t_gpu_line_list line_list;
    line_list.count = 1;
    line_list.m_vao = 0;
    line_list.m_vbo = 0;

    float data[6] = {
        start[0], start[1], start[2],
        end[0], end[1], end[2]
    };

    t_gizmo gizmo;
    gizmo.type = GIZMO_LINE;
    gizmo.persistent = persistent;
    gizmo.colour[0] = colour[0];
    gizmo.colour[1] = colour[1];
    gizmo.colour[2] = colour[2];

    gizmo.line.end[0] = end[0];
    gizmo.line.end[1] = end[1];
    gizmo.line.end[2] = end[2];

    gizmo.line.lines = line_list;

    gizmo_add(engine, gizmo);
}

void gizmo_line_renderer(t_render_ctx* ctx, t_gizmo* gizmo)
{
    float data[6] = {
        gizmo->position[0], gizmo->position[1], gizmo->position[2],
        gizmo->line.end[0], gizmo->line.end[1], gizmo->line.end[2]
    };

    static GLuint vao = 0;
    static GLuint vbo = 0;
    if (vao == 0)
    {
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glGenBuffers(1, &vbo));

        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
    }

    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, data, GL_DYNAMIC_DRAW));

    t_gpu_line_list line_list = (t_gpu_line_list){
        .lines = NULL,
        .count = 1,
        .m_vao = vao,
        .m_vbo = vbo
    };

    t_material* material = material_system_mat_get(&ctx->material_system, "basic_colour_material");

    material_prop_update_named(material, "colour", (t_mat_prop_value){.f3 = {gizmo->colour[0], gizmo->colour[1], gizmo->colour[2]}});
    material_apply(ctx, material_system_mat_get(&ctx->material_system, "basic_colour_material"));
    {
        t_mat_prop *prop = material_prop_get(material, "MVP");
        if (prop != NULL)
        {
            glm_mat4_identity(prop->value.mat);
            glm_mat4_mul(ctx->camera.premultPV, prop->value.mat, prop->value.mat);
            material_prop_update(material, prop);
        }
    }

    render_gpu_line_list(&line_list);
}

void gizmo_cube(t_engine* engine, vec3 position, vec3 scale, vec3 colour, bool persistent)
{
    t_gizmo gizmo;
    gizmo.type = GIZMO_CUBE;
    gizmo.persistent = persistent;
    gizmo.colour[0] = colour[0];
    gizmo.colour[1] = colour[1];
    gizmo.colour[2] = colour[2];
    gizmo.position[0] = position[0];
    gizmo.position[1] = position[1];
    gizmo.position[2] = position[2];
    gizmo.cube.scale[0] = scale[0];
    gizmo.cube.scale[1] = scale[1];
    gizmo.cube.scale[2] = scale[2];

    gizmo_add(engine, gizmo);
}

void gizmo_cube_render(t_render_ctx* ctx, t_gizmo* gizmo)
{
    static t_gpu_mesh* cube_mesh = NULL;

    if (cube_mesh == NULL)    {
        t_mesh mesh_cube;
        mesh_create(&mesh_cube);
        vec_vec3_push_back(&mesh_cube.vertices, (t_vec3){.v = {-0.5f, -0.5f, -0.5f}});
        vec_vec3_push_back(&mesh_cube.vertices, (t_vec3){.v = {0.5f, -0.5f, -0.5f}});
        vec_vec3_push_back(&mesh_cube.vertices, (t_vec3){.v = {0.5f, 0.5f, -0.5f}});
        vec_vec3_push_back(&mesh_cube.vertices, (t_vec3){.v = {-0.5f, 0.5f, -0.5f}});
        vec_vec3_push_back(&mesh_cube.vertices, (t_vec3){.v = {-0.5f, - 0.5f, 0.5f}});
        vec_vec3_push_back(&mesh_cube.vertices, (t_vec3){.v = {0.5f, -0.5f, 0.5f}});
        vec_vec3_push_back(&mesh_cube.vertices, (t_vec3){.v = {0.5f, 0.5f,  0.5f}});
        vec_vec3_push_back(&mesh_cube.vertices, (t_vec3){.v = {-0.5f, 0.5f, 0.5f}}); 
        // indices
        int indices[] = {
            0, 1, 2, 2, 3, 0,
            1, 5, 6, 6, 2, 1,
            5, 4, 7, 7, 6, 5,
            4, 0, 3, 3, 7, 4,
            3, 2, 6, 6, 7, 3,
            4, 5, 1, 1, 0, 4
        };
        for (int i = 0; i < 36; i++)
            vec_GPUIndex_push_back(&mesh_cube.indices, indices[i]); 

        cube_mesh = gpu_mesh_add(&mesh_cube);
    }

    
    t_material* material = material_system_mat_get(&ctx->material_system, "basic_colour_material");

    material_prop_update_named(material, "colour", (t_mat_prop_value){.f3 = {gizmo->colour[0], gizmo->colour[1], gizmo->colour[2]}});
    material_apply(ctx, material_system_mat_get(&ctx->material_system, "basic_colour_material"));

    {
        t_mat_prop *prop = material_prop_get(material, "MVP");
        if (prop != NULL)
        {
            glm_mat4_identity(prop->value.mat);
            glm_translate(prop->value.mat, gizmo->position);
            glm_scale(prop->value.mat, gizmo->cube.scale);
            glm_mat4_mul(ctx->camera.premultPV, prop->value.mat, prop->value.mat);
            material_prop_update(material, prop);
        }
    }

    GLCall(glBindVertexArray(cube_mesh->m_vao));
    GLCall(glDrawElements(GL_TRIANGLES, cube_mesh->m_size, GL_UNSIGNED_INT, 0));
}

void render_gizmos_array(t_render_ctx* ctx, t_gizmo_array* arr)
{
    for (size_t i = 0; i < arr->_len; i++)
    {
        t_gizmo* gizmo = &arr->data[i];
        switch (gizmo->type)
        {
            case GIZMO_LINE:
                gizmo_line_renderer(ctx, gizmo);
                break;
            case GIZMO_CUBE:
                gizmo_cube_render(ctx, gizmo);
                break;
            case GIZMO_SPHERE:
                break;
        }
    }
}

void render_gizmos(t_engine* engine)
{
    t_gizmo_register* gizmos = *named_register_get(engine, "gizmo_register");
   
    render_gizmos_array(&engine->render_context, &gizmos->gizmo_persist);
    render_gizmos_array(&engine->render_context, &gizmos->gizmo_frame);
    gizmos->gizmo_frame._len = 0;
}