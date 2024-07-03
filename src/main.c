#include "load_glad.h"
#include <GLFW/glfw3.h>
#include "io.h"
#include "asset.h"

#include <string.h>
#include <stdio.h>
#include "shader.h"
#include "model.h"
#include "gldebug.h"
#include "gpu/mesh.h"
#include "material.h"
#include "material_system.h"
#include "engine.h"
#include "entity.h"
#include "renderer.h"

void materials_setup(t_material_system *system)
{
    char path[PATH_MAX];
    
    path[0] = '\0';
    asset_get_path(path, 2, "shaders", "basic_vertex.glsl");
    char *basic_vertex = strdup(path);
    path[0] = '\0';
    asset_get_path(path, 2, "shaders", "basic_fragment.glsl");
    char *basic_fragment = strdup(path);

	GLuint program;
    gpu_shader_program_compile_vert_frag(basic_vertex, basic_fragment, &program);
	t_shader *shader = material_system_shader_add(system, (t_shader){.shader_id = program, .name = "basic_shader"});	
	t_material *basic = material_system_mat_create(system, (t_material){.shader = shader, .name = "basic_material"});

	vec_mat_prop props = {};
	vec_mat_prop_push(&props, (t_mat_prop){.name = "MVP", .type=MPT_MAT4});
	material_prop_add_new(basic, "MVP", MPT_MAT4, MPT_DEFAULT);
	material_prop_add_new(basic, "colour", MPT_FLOAT3, (t_mat_prop_value){.f3={1,1,0}});

	free(basic_fragment);
	free(basic_vertex);
}

int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        perror("Failed to initialize GLAD");
        return -1;
    }
	t_engine engine = {0};

    materials_setup(&engine.material);

    char path[PATH_MAX];
    t_model *crate = model_load(asset_get_path(path, 2, "models", "crate.obj"));

    t_gpu_mesh *mesh = gpu_mesh_add(&crate->mesh); 
    (void)mesh;
	

	t_entity *ent = entity_create(ET_BASE);
	ent->renderer = mesh_renderer_create(mesh, material_system_mat_get(&engine.material, "basic_material"));
	entity_render_attach(&engine.render_context, ent);

    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClearColor(.2,.2,.6,1));
        glClear(GL_COLOR_BUFFER_BIT);

		render_entities(&engine.render_context);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}