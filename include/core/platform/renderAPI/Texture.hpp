// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include "utils/utils.hpp"
#include <string>

enum class Tex_WarppingMode
{
    REPEAT,
    CLAMP,
};

enum class Tex_FilteringMode
{
    NEAREST,
    LINEAR,
    Mipmap,
};

class Texture2D
{
protected:
    Tex_WarppingMode m_wpm;
    Tex_FilteringMode m_ftm;
    uint32_t m_channels;
    int m_width = 1;
    int m_height = 1;
    std::string m_path;

public:
    virtual void resize(int width, int height) = 0;
    virtual void set_sample(Tex_WarppingMode wpm, Tex_FilteringMode ftm) = 0;
    virtual Texture2D &operator=(Ref<Image> image) = 0;
    virtual Texture2D &set_image(Ref<Image> image) = 0;
    virtual void gen_mipmap() = 0;

    Texture2D() {};

    void init(Tex_WarppingMode wpm = Tex_WarppingMode::REPEAT, Tex_FilteringMode ftm = Tex_FilteringMode::LINEAR, uint32_t channels = Tex::Channels::None)
    {
        set_sample(wpm, ftm);
        set_channels(channels);
    }

    const std::string &get_path()
    {
        return m_path;
    }

    void set_channels(uint32_t channels)
    {
        m_channels = channels;
        if (m_channels != Tex::Channels::None)
            resize(m_width, m_height);
    }

    uint32_t get_channels()
    {
        return m_channels;
    }

    // return (width,height)
    glm::vec2 get_size()
    {
        return glm::vec2(m_width, m_height);
    }

    virtual ~Texture2D() {};
};
