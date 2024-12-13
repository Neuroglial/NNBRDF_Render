// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include "utils/utils.hpp"
#include <string>

enum class Tex_Channels : int32_t
{
    None,
    R = 0x2002,      // GL_R
    RG = 0x8227,     // GL_RG
    RGB = 0x1907,    // GL_RGB
    RGBA = 0x1908,   // GL_RGBA
    RGB16F = 0x881B, // GL_RGB16F
    RGB32F = 0x8815, // GL_RGB32F
};

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
    Tex_WarppingMode m_wpm = Tex_WarppingMode::REPEAT;
    Tex_FilteringMode m_ftm = Tex_FilteringMode::LINEAR;
    Tex_Channels m_channels = Tex_Channels::RGB;
    int m_width = 1;
    int m_height = 1;
    std::string m_path;

public:
    virtual void resize(int width, int height) = 0;
    virtual void set_sample(Tex_WarppingMode wpm, Tex_FilteringMode ftm) = 0;
    virtual Texture2D &operator=(Ref<Image> image) = 0;
    virtual Texture2D &set_image(Ref<Image> image) = 0;
    virtual void gen_mipmap() = 0;

    Texture2D(Tex_WarppingMode wpm, Tex_FilteringMode ftm) : m_wpm(wpm), m_ftm(ftm) {}

    Texture2D(Tex_WarppingMode wpm, Tex_FilteringMode ftm, Ref<Image> image) : Texture2D(wpm, ftm) {}

    Texture2D(int width, int height, Tex_Channels channels = Tex_Channels::RGB, Tex_WarppingMode wpm = Tex_WarppingMode::REPEAT, Tex_FilteringMode ftm = Tex_FilteringMode::LINEAR)
        : m_width(width), m_height(height), m_wpm(wpm), m_ftm(ftm), m_channels(channels) {}

    Texture2D() {

    };

    const std::string &get_path()
    {
        return m_path;
    }

    void set_channels(Tex_Channels channels)
    {
        m_channels = channels;
        resize(m_width, m_height);
    }

    Tex_Channels get_channels()
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
