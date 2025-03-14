// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include <glm/glm.hpp>

class Base
{
public:
    virtual void viewport(int bx, int by, int width, int height) = 0;
    virtual void get_frameBufferSize(int &width, int &height) = 0;
    virtual void clear(const glm::vec4 &color = glm::vec4(0, 0, 0, 1)) = 0;
    virtual void depth_test(bool enable) = 0;
    virtual void face_culling(bool enable, bool back_culling) = 0;
    virtual void init() = 0;
    virtual void unbindFrameBuffer() = 0;
};