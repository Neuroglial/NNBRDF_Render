#pragma once

#include <glm/glm.hpp>
#include "core/platform/renderAPI/RenderAPI.hpp"

template <typename T,int binding>
class UniformManager
{
public:
    static T &GetData()
    {
        static T data;
        return data;
    }

    static Ref<UniformBuffer> GetUniform()
    {
        static Ref<UniformBuffer> uniform;
        if (uniform == nullptr)
        {
            uniform = RenderAPI::creator<UniformBuffer>::crt();
            uniform->reset(sizeof(T), binding);
        }

        return uniform;
    }

    static void UpdataBuffer()
    {
        GetUniform()->set_data(0, sizeof(T), &GetData());
    }
};