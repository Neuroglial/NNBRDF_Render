// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include "core/platform/renderAPI/Texture.hpp"
#include <map>

class FrameBuffer
{
public:
    enum AttachType
    {
        Color,
        DepthStencil
    };

    virtual void attach(Ref<Texture2D> &tex, AttachType type, int index) = 0;
    virtual void clear() = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;

    FrameBuffer(int width = -1, int height = -1) : m_width(width), m_height(height) {}

    virtual ~FrameBuffer() {}

    virtual void resize(int width,int height) = 0;

protected:
    std::map<int,Ref<Texture2D>> m_attachs;
    int m_width = 0;
    int m_height = 0;
};