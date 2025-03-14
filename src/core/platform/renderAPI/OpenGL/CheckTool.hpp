#pragma once
#include <iostream>
#include <stdexcept>

#define GL_Chack_Enable
#ifdef GL_Chack_Enable
#define GL_Check()                                               \
    {                                                            \
        GLenum error = glGetError();                             \
        if (error != GL_NO_ERROR)                                \
        {                                                        \
            std::cout << "OpenGL Error: " << error << std::endl; \
            throw std::runtime_error("Error");                   \
        }                                                        \
    }

#define GL_Check_Throw()                                     \
    GLenum error = glGetError();                             \
    if (error != GL_NO_ERROR)                                \
    {                                                        \
        std::cout << "OpenGL Error: " << error << std::endl; \
        throw std::runtime_error("Error");                   \
    }

#else
#define GL_Check()
#endif
