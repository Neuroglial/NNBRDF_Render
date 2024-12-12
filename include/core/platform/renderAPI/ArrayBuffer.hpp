#pragma once
#include <vector>

template <typename T>
class ArrayBuffer
{
public:
    virtual void set_data(const std::vector<T> &data) = 0;

    virtual void set_data(std::vector<T> &&data) = 0;

    int size()
    {
        return m_data.size();
    }

    const std::vector<T> &get_buffer()
    {
        return m_data;
    }

    virtual ~ArrayBuffer() {};

protected:
    std::vector<T> m_data;
};