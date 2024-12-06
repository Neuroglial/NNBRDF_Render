#pragma once

class UniformBuffer
{
public:
    virtual void set_data(int offset, int size, void *data) = 0;
    virtual void resize(int size) = 0;
    virtual void bind(int index) = 0;

    UniformBuffer(int size, int index) : m_size(size), m_index(index) {}

    UniformBuffer(int size) : m_size(size), m_index(-1) {}

    UniformBuffer() : m_size(0), m_index(-1) {}

protected:
    int m_size;
    int m_index;
};