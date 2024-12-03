#pragma once
#include <vector>

template<typename T>
class ArrayBuffer{
    public:
    virtual void set_data(const std::vector<T>& data) = 0;

    virtual ~ArrayBuffer(){};

    protected:
    std::vector<T> m_data;
};