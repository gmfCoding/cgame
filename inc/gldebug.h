#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#define BREAKFUNC DebugBreak();
#include <debugapi.h>
#elif __linux__ 
#include <signal.h>

#ifdef SIGTRAP
#define BREAKFUNC raise(SIGTRAP);
#endif
#endif

#include "logging.h"

#define ASSERT(x) if(!(x)) BREAKFUNC;

// #define NO_GL_DEBUG

// #ifdef NO_GL_DEBUG
// #define GLCall(x) x
// #else

#define GLCall(x) GLClearLog(); te_logf(LOG_LEVEL_INFO, "glCall", "Calling OpenGL function: %s", #x); \
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__));\

#define GLCall_Extra(x) GLCall(x)

// #endif

static void GLClearLog()
{
    while(glGetError() != GL_NO_ERROR);
}

static const char* GLGetError(int error)
{
    switch (error)
    {
    case GL_INVALID_ENUM: 
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE: 
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION: 
        return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION: 
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY: 
        return "GL_OUT_OF_MEMORY";
    case GL_STACK_UNDERFLOW: 
        return "GL_STACK_UNDERFLOW";
    case GL_STACK_OVERFLOW: 
        return "GL_STACK_OVERFLOW";
    case GL_NO_ERROR: 
    default:
        return "GL_NO_ERROR";
    }
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    GLenum error;
    //printf("[OpenGL Call] %s %s:%d\n", function, file, line);

    while ((error = glGetError()))
    {
        printf("[OpenGL Error] (%s). %s %s:%d\n", GLGetError((int)error), function, file, line);
        return false;
    }
    return true;
}