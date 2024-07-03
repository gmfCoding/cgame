#include "load_glad.h"
#include <stdio.h>
#include "io.h"
#include "gldebug.h"
#include <alloca.h>
#include <stdlib.h>

void gpu_shader_debug_shader(GLuint shader)
{
    GLint result = GL_FALSE;
    int logLength;
    GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
    GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength));
    char *shaderError = alloca((logLength > 1) ? logLength : 1);
    shaderError[0] = '\0';
    GLCall(glGetShaderInfoLog(shader, logLength, NULL, shaderError));
    printf("%s\n", shaderError);
}

void gpu_shader_debug_program(GLuint program)
{
    GLint result = GL_FALSE;
    int logLength;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
    GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength));
    char *programError = alloca((logLength > 1) ? logLength : 1);
    programError[0] = '\0';
    GLCall(glGetProgramInfoLog(program, logLength, NULL, programError));
    printf("%s\n", programError);
}

/// @brief Compiles vert and frag into a shader program
/// @param vert path to the vertex shader
/// @param frag path to the frament shader
/// @param program pointer to GLuint to store the output 
/// @return 0 on success and 1 on failure
#include <string.h>
int gpu_shader_program_compile_vert_frag(const char* vert, const char *frag, GLuint* program)
{
    char *vert_source = io_fileread(vert);
    char *frag_source = io_fileread(frag);

    const char *vert_source2[1];
    const char *frag_source2[1];
    vert_source2[0] = vert_source;
    frag_source2[0] = frag_source;

    if (vert_source == NULL && frag_source != NULL)
        free((void*)frag_source);
    if (frag_source == NULL && vert_source != NULL)
        free((void*)vert_source);
    if (frag_source == NULL || vert_source == NULL)
        return 1;

    GLCall(GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(vert_shader, 1, vert_source2, NULL));
    GLCall(glCompileShader(vert_shader));
    gpu_shader_debug_shader(vert_shader);

    GLCall(GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(frag_shader, 1, frag_source2, NULL));
    GLCall(glCompileShader(frag_shader));
    gpu_shader_debug_shader(frag_shader);

    GLCall(GLuint program_id = glCreateProgram());
    GLCall(glAttachShader(program_id, vert_shader));
    GLCall(glAttachShader(program_id, frag_shader));
    GLCall(glLinkProgram(program_id));

    gpu_shader_debug_program(program_id);


    GLCall(glDeleteShader(vert_shader));
    GLCall(glDeleteShader(frag_shader));
    if (program != NULL)
        *program = program_id;
    return program == NULL;
}