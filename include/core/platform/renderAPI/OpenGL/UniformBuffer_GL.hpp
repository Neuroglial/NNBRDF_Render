#pragma once
#include "core/platform/renderAPI/UniformBuffer.hpp"


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

    UniformBuffer_GL():UniformBuffer() {}

    unsigned int get_id()
    {
        return m_id;
    }

    virtual void set_data(int offset, int size, void *data) override;

    virtual void resize(int size) override;

    virtual void bind(int index) override;

private:
    unsigned int m_id = 0;
};