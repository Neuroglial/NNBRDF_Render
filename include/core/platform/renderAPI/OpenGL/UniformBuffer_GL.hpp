#pragma once
#include "core/platform/renderAPI/UniformBuffer.hpp"
#include <glad/glad.h>

class UniformBuffer_GL : public UniformBuffer
{
public:
    UniformBuffer_GL(int size, int index) : UniformBuffer(size, index)
    {
        resize(size);
        bind(index);
    }

    UniformBuffer_GL(int size) : UniformBuffer(size)
    {
        resize(size);
    }

    UniformBuffer_GL() {}

    unsigned int get_id()
    {
        return m_id;
    }

    virtual void set_data(int offset, int size, void *data) override
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
    virtual void resize(int size) override
    {
        m_size = size;
        if (m_id)
            glDeleteBuffers(1, &m_id);

        glGenBuffers(1, &m_id);
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        glBufferData(GL_UNIFORM_BUFFER, m_size, NULL, GL_STATIC_DRAW);
    }
    virtual void bind(int index) override
    {
        m_index = index;
        if (m_id)
        {
            glBindBuffer(GL_UNIFORM_BUFFER, m_id);
            glBindBufferBase(GL_UNIFORM_BUFFER, m_index, m_id);
        }
        else
        {
            throw std::runtime_error("Uniform Buffer Binding Error");
        }
    }

private:
    unsigned int m_id = 0;
};