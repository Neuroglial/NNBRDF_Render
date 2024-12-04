#pragma once
#include <vector>

template<typename T>
class ArrayBuffer{
    public:
    virtual void set_data(const std::vector<T>& data) = 0;
    int size(){
        return m_data.size();
    }

    virtual ~ArrayBuffer(){};

    protected:
    std::vector<T> m_data;
};