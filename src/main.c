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

GLuint materials_setup()
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
    return program;
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

    GLuint program = materials_setup();
    char path[PATH_MAX];
    t_model *crate = model_load(asset_get_path(path, 2, "models", "crate.obj"));

    t_gpu_mesh *mesh = gpu_mesh_add(&crate->mesh); 
    (void)mesh;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(.2,.2,.6,1);
        glClear(GL_COLOR_BUFFER_BIT);
        (void)program;
        GLCall(glUseProgram(program));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}