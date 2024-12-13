// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include "core/platform/renderAPI/FrameBuffer.hpp"

class FrameBuffer_GL : FrameBuffer
{
public:
    FrameBuffer_GL(int width = -1, int height = -1) : FrameBuffer(width, height) {}

    virtual void resize(int width, int height) override;

    virtual void attach(Ref<Texture2D> &tex, AttachType type, int index = -1) override;

    virtual void clear() override;

    virtual void bind() override;

    virtual void unbind() override;

    virtual ~FrameBuffer_GL();

private:
    unsigned int m_id = 0;
    bool m_depth_attach = false;
    unsigned int m_renderbuffer_id = 0;
    bool m_complete = false;
};