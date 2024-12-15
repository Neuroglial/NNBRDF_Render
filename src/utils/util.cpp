// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "utils/utils.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace std
{
    string to_string(const glm::vec2 &val)
    {
        return "(" + std::to_string(val.x) + "," + std::to_string(val.y) + ")";
    }
    string to_string(const glm::vec3 &val)
    {
        return "(" + std::to_string(val.x) + "," + std::to_string(val.y) + "," + std::to_string(val.z) + ")";
    }

    string to_string(const glm::vec4 &val)
    {
        return "(" + std::to_string(val.x) + "," + std::to_string(val.y) + "," + std::to_string(val.z) + "," + std::to_string(val.w) + ")";
    }
    string to_string(const glm::mat2 &val)
    {
        return to_string(val[0]) + "\n" + to_string(val[1]);
    }
    string to_string(const glm::mat3 &val)
    {
        return to_string(val[0]) + "\n" + to_string(val[1]) + "\n" + to_string(val[2]);
    }
    string to_string(const glm::mat4 &val)
    {
        return to_string(val[0]) + "\n" + to_string(val[1]) + "\n" + to_string(val[2]) + "\n" + to_string(val[3]);
    }
}

std::string read_from_file(const std::string &path)
{
    std::string readed;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path.c_str());
    std::stringstream stream;
    stream << file.rdbuf();

    file.close();
    readed = stream.str();

    return readed;
}

Ref<Image> read_image(const std::string &path, bool flip_vertically)
{
    Ref<Image> img = std::make_shared<Image>();

    stbi_set_flip_vertically_on_load(flip_vertically);

    int channels;
    img->m_data = stbi_load(path.c_str(), &img->m_width, &img->m_height, &channels, 0);

    img->m_channels = (Tex::Channels)channels;
    img->m_channels |= Tex::Channels::UI;

    if (!img->m_data)
        throw std::runtime_error("Image Read Error");

    return img;
}

Ref<Image> read_image_hdr(const std::string &path, bool flip_vertically)
{
    Ref<Image> img = std::make_shared<Image>();

    stbi_set_flip_vertically_on_load(flip_vertically);

    int channels;
    img->m_data = stbi_loadf(path.c_str(), &img->m_width, &img->m_height, &channels, 0);

    img->m_channels = (Tex::Channels)channels;
    img->m_channels |= Tex::Channels::Bit32;

    if (!img->m_data)
        throw std::runtime_error("Image Read Error");

    return img;
}

Image::~Image()
{
    if (m_data != nullptr)
        stbi_image_free(m_data);
}

namespace utils
{
    glm::mat4 get_rotate(const glm::vec3 &degree, glm::mat4 mat)
    {

        mat = glm::rotate_slow(mat, degree.y, glm::vec3(0.0f, 1.0f, 0.0f));
        mat = glm::rotate_slow(mat, degree.x, glm::vec3(1.0f, 0.0f, 0.0f));
        mat = glm::rotate_slow(mat, degree.z, glm::vec3(0.0f, 0.0f, 1.0f));

        return mat;
    }

    glm::mat4 get_model(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rotation, glm::mat4 mat)
    {
        mat = glm::translate(mat, pos);
        mat = glm::scale(mat, scale);
        mat = get_rotate(rotation, mat);

        return mat;
    }
}