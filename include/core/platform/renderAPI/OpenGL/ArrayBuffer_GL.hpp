#pragma once
#include "core/platform/renderAPI/ArrayBuffer.hpp"
#include <glad/glad.h>


template <typename T>
class ArrayBuffer_GL:public ArrayBuffer<T>{
    public:
    virtual void set_data(const std::vector<T>& data) override
    {
        if(m_id)
            glDeleteBuffers(1,&m_id);

        ArrayBuffer<T>::m_data = data;

        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*data.size(), data.data(), GL_STATIC_DRAW);
        
    }

    unsigned int get_id() {return m_id;}


    ArrayBuffer_GL():m_id(0){}

    virtual ~ArrayBuffer_GL(){
        if(m_id)
            glDeleteBuffers(1,&m_id);
    }

    private:
    unsigned int m_id;

};