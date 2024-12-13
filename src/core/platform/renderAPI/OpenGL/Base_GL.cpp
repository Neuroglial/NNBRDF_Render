#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/platform/renderAPI/OpenGL/Base_GL.hpp"
#include <stdexcept>
#include "core/platform/renderAPI/RenderAPI.hpp"

REGISTER_API(Base_GL)

void Base_GL::viewport(int bx, int by, int width, int height)
{
    glViewport(bx, by, width, height);
}

void Base_GL::clear()
{
    glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Base_GL::depth_test(bool enable)
{
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void Base_GL::face_culling(bool enable, bool back_culling)
{
    if (enable)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    if (back_culling)
        glCullFace(GL_BACK);
    else
        glCullFace(GL_FRONT);
}

void Base_GL::init()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}