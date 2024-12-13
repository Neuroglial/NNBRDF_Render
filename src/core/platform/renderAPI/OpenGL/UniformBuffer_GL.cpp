// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/platform/renderAPI/OpenGL/UniformBuffer_GL.hpp"
#include <glad/glad.h>
#include "core/platform/renderAPI/RenderAPI.hpp"
#include <stdexcept>

REGISTER_API(UniformBuffer_GL)

void UniformBuffer_GL::set_data(int offset, int size, void *data)
{
    if (m_id && (offset + size) <= m_size)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    }
    else
    {
        throw std::runtime_error("Uniform Buffer Set Data Error");
    }
}

void UniformBuffer_GL::resize(int size)
{
    m_size = size;
    if (m_id)
        glDeleteBuffers(1, &m_id);

    glGenBuffers(1, &m_id);
    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    glBufferData(GL_UNIFORM_BUFFER, m_size, NULL, GL_STATIC_DRAW);
}

void UniformBuffer_GL::bind(int binding)
{
    m_binding = binding;
    if (m_id)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_id);
    }
    else
    {
        throw std::runtime_error("Uniform Buffer Binding Error");
    }
}