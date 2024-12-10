#pragma once

class UniformBuffer
{
public:
    virtual void set_data(int offset, int size, void *data) = 0;
    virtual void resize(int size) = 0;
    virtual void bind(int binding) = 0;

    void reset(int size,int binding)
    {
        resize(size);
        bind(binding);
    }

    UniformBuffer(int size, int binding) : m_size(size), m_binding(binding) {}

    UniformBuffer(int size) : m_size(size), m_binding(-1) {}

    UniformBuffer() : m_size(0), m_binding(-1) {}

protected:
    int m_size;
    int m_binding;
};