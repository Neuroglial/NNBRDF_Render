// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include "core/platform/renderAPI/FrameBuffer.hpp"

class FrameBuffer_GL : FrameBuffer
{
public:
    FrameBuffer_GL();

    virtual void resize(int width, int height) override;

    virtual void attach(Ref<Texture2D> &tex, int index) override;
    virtual void attach(Ref<TextureCube> &tex, int index) override;

    virtual void bind() override;

    virtual void unbind() override;

    virtual ~FrameBuffer_GL();

private:
    unsigned int m_id = 0;
    unsigned int m_renderbuffer_id = 0;
    bool m_complete = false;

    friend class Base_GL;
};