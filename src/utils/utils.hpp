// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <stdexcept>
#include <vector>
#include <map>
#include <list>

#define Root_Path std::string("../../")

#define Path(x) (Root_Path + x)

#define Log(x) std::cout << "Log File: " << std::string(__FILE__) << " Line: " << std::to_string(__LINE__) << "\n" \
                         << x << std::endl;

#define Assert(x) \
    if (!(x))     \
    throw std::runtime_error(std::string("ASSERT ERROR File: " + std::string(__FILE__) + " Line: " + std::to_string(__LINE__)).c_str())

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using Wk = std::weak_ptr<T>;

enum RendererType
{
    None,
    OpenGL,
};

namespace Tex
{
    enum Channels : uint32_t
    {
        // 0x 00-00-00-00
        CL_MASK = 0xF,
        R = 0x1,
        RG = 0x2,
        RGB = 0x3,
        RGBA = 0x4,

        Bit_MASK = 0xF0,
        Bit8 = 0x00, // default
        Bit16 = 0x10,
        Bit32 = 0x20,

        Type_Mask = 0xF00,
        F = 0x000,  // float default
        I = 0x100,  // int
        UI = 0x200, // unsigned bit

        Special_Mask = 0xF000,
        Color = 0x0000, // default
        Depth = 0x1000,

        None = 0xF0000,
    };

    enum WarppingMode
    {
        REPEAT,
        CLAMP,
    };

    enum FilteringMode
    {
        NEAREST,
        LINEAR,
        Mipmap,
    };

    inline std::string to_string(WarppingMode warp)
    {
        switch (warp)
        {
        case REPEAT:
            return "repeat";
            break;

        case CLAMP:
            return "clamp";
            break;

        default:
            break;
        }

        return "none";
    }

    inline WarppingMode to_warp(const std::string &str)
    {
        if (str == "repeat")
            return REPEAT;

        return CLAMP;
    }

    inline std::string to_string(FilteringMode filter)
    {
        switch (filter)
        {
        case NEAREST:
            return "nearest";
            break;

        case LINEAR:
            return "linear";
            break;

        case Mipmap:
            return "mipmap";
            break;

        default:
            break;
        }

        return "none";
    }

    inline FilteringMode to_filter(const std::string &str)
    {
        if (str == "nearest")
            return NEAREST;
        else if (str == "linear")
            return LINEAR;

        return Mipmap;
    }
}

struct Image
{
    int m_width;
    int m_height;
    void *m_data;
    uint32_t m_channels;

    std::string m_path;

    bool m_yflip;

    Image() : m_width(0), m_height(0), m_data(nullptr), m_channels(Tex::Channels::None) {}

    ~Image();
};

namespace std
{
    string to_string(const glm::vec2 &val);
    string to_string(const glm::vec3 &val);
    string to_string(const glm::vec4 &val);
    string to_string(const glm::mat2 &val);
    string to_string(const glm::mat3 &val);
    string to_string(const glm::mat4 &val);
}

namespace utils
{
    std::string read_from_file(const std::string &path);
    std::string read_from_file_with_include(const std::string &path);
    std::vector<std::string> get_between(const std::string &str, const std::string &first, const std::string &second);

    Ref<Image> read_image(const std::string &path, bool flip_vertically = true);
    Ref<Image> get_color_Image(glm::vec4 color, uint32_t channels);
    Ref<Image> read_image_hdr(const std::string &path, bool flip_vertically = true);

    glm::vec3 to_euler(const glm::quat &q);
    glm::quat to_quat(const glm::vec3 &degrees);

    glm::mat4 get_rotation(const glm::quat &q);
    inline glm::mat4 get_rotation(const glm::vec3 &degree, glm::mat4 mat = glm::mat4(1)) { return mat * get_rotation(to_quat(degree)); }

    glm::mat4 get_model(const glm::vec3 &pos, const glm::vec3 &scale, const glm::quat &rotation, glm::mat4 mat = glm::mat4(1));
    inline glm::mat4 get_model(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rotation, glm::mat4 mat = glm::mat4(1)) { return get_model(pos, scale, to_quat(rotation), mat); }

    glm::vec3 get_position(const glm::mat4 &model);
    glm::quat get_rotation(const glm::mat4 &model);
    glm::vec3 get_scale(const glm::mat4 &model);

    glm::vec3 get_right(const glm::mat4 &model);
    glm::vec3 get_forword(const glm::mat4 &model);

    inline glm::vec3 forward() { return glm::vec3(0, 0, 1); }
    inline glm::vec3 right() { return glm::vec3(-1, 0, 0); }
}