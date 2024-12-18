// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include "core/platform/renderAPI/Texture.hpp"
#include <glm/glm.hpp>
#include <map>

class FrameBuffer
{
public:
    virtual void attach(Ref<Texture2D> &tex, int index) = 0;
    virtual void attach(Ref<TextureCube> &tex, int index) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;

    glm::vec2 get_size()
    {
        return glm::vec2(m_width, m_height);
    }

    void bind(const glm::vec4 &clear_color);

    FrameBuffer() {}

    virtual ~FrameBuffer() {}

    void init(int width, int height);

    virtual void resize(int width, int height) = 0;

protected:
    std::map<int, Ref<Texture2D>> m_attachs_2D;
    std::map<int, Ref<TextureCube>> m_attachs_cube;
    Ref<Texture2D> m_depth_2D;
    Ref<TextureCube> m_depth_cube;
    int m_width;
    int m_height;
};