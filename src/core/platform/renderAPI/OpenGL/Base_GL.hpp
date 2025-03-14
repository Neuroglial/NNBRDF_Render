// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include "core/platform/renderAPI/Base.hpp"

class Base_GL : Base
{
public:
    virtual void viewport(int bx, int by, int width, int height) override;

    virtual void clear(const glm::vec4 &color) override;

    virtual void depth_test(bool enable) override;

    virtual void face_culling(bool enable, bool back_culling) override;

    virtual void init() override;

    virtual void get_frameBufferSize(int &width, int &height) override;

    virtual void unbindFrameBuffer() override;
};