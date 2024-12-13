#pragma once
#include "core/platform/renderAPI/Base.hpp"

class Base_GL : Base
{
public:
    virtual void viewport(int bx, int by, int width, int height) override;

    virtual void clear() override;

    virtual void depth_test(bool enable) override;
    
    virtual void face_culling(bool enable, bool back_culling) override;

    virtual void init() override;
};