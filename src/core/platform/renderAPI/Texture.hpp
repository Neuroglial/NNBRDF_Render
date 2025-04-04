// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include "utils/utils.hpp"
#include <string>
#include "scene/ImageManager.hpp"

class Texture2D
{
protected:
    Tex::WarppingMode m_wpm = Tex::WarppingMode::REPEAT;
    Tex::FilteringMode m_ftm = Tex::FilteringMode::LINEAR;
    uint32_t m_channels = Tex::Channels::None;

    bool m_yflip;

    int m_width = 1;
    int m_height = 1;
    std::string m_path;

public:
    virtual void resize(int width, int height) = 0;
    virtual void set_sample(Tex::WarppingMode wpm, Tex::FilteringMode ftm) = 0;
    virtual Texture2D &operator=(Ref<Image> image) = 0;
    virtual void set_image(Ref<Image> image) = 0;
    virtual void gen_mipmap() = 0;
    virtual unsigned int get_id() = 0;
    

    Texture2D() {};

    void init(Tex::WarppingMode wpm = Tex::WarppingMode::REPEAT, Tex::FilteringMode ftm = Tex::FilteringMode::LINEAR, uint32_t channels = Tex::Channels::None)
    {
        set_sample(wpm, ftm);
        set_channels(channels);
    }

    const std::string &get_path()
    {
        return m_path;
    }

    Tex::FilteringMode get_filter()
    {
        return m_ftm;
    }

    bool get_yflip()
    {
        return m_yflip;
    }

    Tex::WarppingMode get_warp()
    {
        return m_wpm;
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
    glm::ivec2 get_size()
    {
        return glm::ivec2(m_width, m_height);
    }

    virtual ~Texture2D() {};
};

class TextureCube
{
protected:
    Tex::WarppingMode m_wpm = Tex::WarppingMode::CLAMP;
    Tex::FilteringMode m_ftm = Tex::FilteringMode::LINEAR;
    uint32_t m_channels = Tex::Channels::None;

    int m_width = 0;
    int m_height = 0;

    bool m_hdr = false;
    std::string m_path;

public:
    TextureCube() {};
    virtual void set_sample(Tex::WarppingMode wpm, Tex::FilteringMode ftm) = 0;
    virtual void resize(int width, int height) = 0;
    virtual void set_subImage(int index, Ref<Image> image) = 0;
    virtual void gen_mipmap() = 0;

    void set_cubemap(const std::string &path, bool hdr = false)
    {
        m_hdr = hdr;

        auto lastDot = path.find_last_of('.');
        auto name = path.substr(0, lastDot);
        auto tail = path.substr(lastDot, path.size() - lastDot);

        m_path = path;

        if (hdr)
        {
            set_subImage(0, ImageManager::get_hdr(path, false));
            for (int i = 1; i < 6; ++i)
            {
                set_subImage(i, ImageManager::get_hdr(name + "_" + std::to_string(i) + tail, false));
            }
        }
        else
        {
            set_subImage(0, ImageManager::get(path, false));
            for (int i = 1; i < 6; ++i)
            {
                set_subImage(i, ImageManager::get(name + "_" + std::to_string(i) + tail, false));
            }
        }
    }

    bool is_hdr()
    {
        return m_hdr;
    }

    const std::string &get_path()
    {
        return m_path;
    }

    Tex::FilteringMode get_filter()
    {
        return m_ftm;
    }

    Tex::WarppingMode get_warp()
    {
        return m_wpm;
    }

    void set_image(Ref<Image> image)
    {
        for (int i = 1; i < 6; ++i)
            set_subImage(i, image);
    }

    void init(Tex::WarppingMode wpm = Tex::WarppingMode::CLAMP, Tex::FilteringMode ftm = Tex::FilteringMode::LINEAR, uint32_t channels = Tex::Channels::None)
    {
        set_sample(wpm, ftm);
        set_channels(channels);
    }

    void set_channels(uint32_t channels)
    {
        m_channels = channels;
        if (m_channels != Tex::Channels::None)
            resize(m_width, m_height);
    }

    glm::ivec2 get_size()
    {
        return glm::ivec2(m_width, m_height);
    }

    uint32_t get_channels()
    {
        return m_channels;
    }
};
