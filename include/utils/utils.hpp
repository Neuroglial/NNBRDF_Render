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

#define Root_Path std::string("../../")

template <typename T>
using Ref = std::shared_ptr<T>;

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
        UI = 0x200, // unsigned int

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
}

struct Image
{
    int m_width;
    int m_height;
    void *m_data;
    uint32_t m_channels;

    std::string m_path;

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

std::string read_from_file(const std::string &path);

Ref<Image> read_image(const std::string &path, bool flip_vertically = true);
Ref<Image> read_image_hdr(const std::string &path, bool flip_vertically = true);

namespace utils
{
    glm::mat4 get_rotation(const glm::vec3 &degree, glm::mat4 mat = glm::mat4(1));

    glm::mat4 get_model(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rotation, glm::mat4 mat = glm::mat4(1));

    glm::vec3 get_position(const glm::mat4& model);
    glm::vec3 get_rotation(const glm::mat4& model);
    glm::vec3 get_scale(const glm::mat4& model);
}