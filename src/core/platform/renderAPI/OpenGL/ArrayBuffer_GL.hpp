// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include "core/platform/renderAPI/ArrayBuffer.hpp"
#include <glad/glad.h>

template <typename T>
class ArrayBuffer_GL : public ArrayBuffer<T>
{
public:
    virtual void set_data(const std::vector<T> &data) override
    {
        m_data = data;
        m_uploaded = false;
    }

    virtual void set_data(std::vector<T> &&data) override
    {
        m_data.swap(data);
        m_uploaded = false;
    }

    virtual void upload() override
    {
        if (!m_uploaded)
        {
            if (m_id)
                glDeleteBuffers(1, &m_id);

            glGenBuffers(1, &m_id);
            glBindBuffer(GL_ARRAY_BUFFER, m_id);
            glBufferData(GL_ARRAY_BUFFER, sizeof(T) * m_data.size(), m_data.data(), GL_STATIC_DRAW);

            m_uploaded = true;
        }
    }

    virtual void bind() override
    {
        if (!m_uploaded)
            upload();

        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    unsigned int get_id() { return m_id; }

    ArrayBuffer_GL() : m_id(0) {}

    virtual ~ArrayBuffer_GL()
    {
        if (m_id)
            glDeleteBuffers(1, &m_id);
    }

private:
    unsigned int m_id;
};