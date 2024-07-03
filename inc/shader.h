#ifndef SHADER_H
#define SHADER_H
#include "load_glad.h"

typedef struct s_shader
{
	const char* name;
    GLuint shader_id;
    const char *vert_path;
    const char *frag_path;
    /* data */
} t_shader;

int gpu_shader_program_compile_vert_frag(const char* vert, const char *frag, GLuint* program);
#endif