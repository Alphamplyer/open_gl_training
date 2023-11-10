#include "renderer.h"

#include <iostream>

void gl_clear_error()
{
    while (glGetError() != GL_NO_ERROR);
}

bool gl_log_call(const char* function, const char* file, int line)
{
    while (const GLenum error = glGetError())
    {
        switch (error)
        {
        case GL_INVALID_ENUM:
            std::cout << "[OpenGL Error] (Invalid Enum) " << function << " " << file << ":" << line << std::endl;
            break;
        case GL_INVALID_VALUE:
            std::cout << "[OpenGL Error] (Invalid Value) " << function << " " << file << ":" << line << std::endl;
            break;
        case GL_INVALID_OPERATION:
            std::cout << "[OpenGL Error] (Invalid Operation) " << function << " " << file << ":" << line << std::endl;
            break;
        case GL_STACK_OVERFLOW:
            std::cout << "[OpenGL Error] (Stack Overflow) " << function << " " << file << ":" << line << std::endl;
            break;
        case GL_STACK_UNDERFLOW:
            std::cout << "[OpenGL Error] (Stack Underflow) " << function << " " << file << ":" << line << std::endl;
            break;
        default:
            std::cout << "[OpenGL Error] (code: " << error << ") " << function << " " << file << ":" << line << std::endl;
            break;
        }

        return false;
    }

    return true;
}