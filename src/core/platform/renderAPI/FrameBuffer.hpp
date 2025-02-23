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

    Ref<Texture2D> get(int i = 0) { return m_attachs_2D[i]; }

    inline float get_aspectRatio() const { return (float)m_width / (float)m_height; }
    inline glm::ivec2 get_size() const { return glm::ivec2(m_width, m_height); }
    static FrameBuffer *get_context() { return get_contextPriv(); }

    void bind(const glm::vec4 &clear_color);

    FrameBuffer() {}

    virtual ~FrameBuffer() {}

    void init(int width, int height);

    virtual void resize(int width, int height) = 0;

    inline void resize(glm::ivec2 size) { resize(size.x, size.y); }

protected:
    static FrameBuffer *&get_contextPriv()
    {
        static FrameBuffer *context;
        return context;
    }

    std::map<int, Ref<Texture2D>> m_attachs_2D;
    std::map<int, Ref<TextureCube>> m_attachs_cube;
    Ref<Texture2D> m_depth_2D;
    Ref<TextureCube> m_depth_cube;
    int m_width = -1;
    int m_height = -1;
};