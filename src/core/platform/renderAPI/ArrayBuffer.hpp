// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include <vector>

template <typename T>
class ArrayBuffer
{
public:
    ArrayBuffer() : m_uploaded(false) {}

    virtual void set_data(const std::vector<T> &data) = 0;

    virtual void set_data(std::vector<T> &&data) = 0;

    virtual void upload() = 0;

    virtual void bind() = 0;

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
    bool m_uploaded;
};