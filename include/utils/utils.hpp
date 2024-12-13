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

struct Image
{
    int m_width;
    int m_height;
    unsigned char *m_data;
    int m_channels;

    std::string m_path;

    Image() : m_width(0), m_height(0), m_data(nullptr), m_channels(0) {}

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

namespace utils
{
    glm::mat4 get_rotate(const glm::vec3 &degree, glm::mat4 mat = glm::mat4(1));

    glm::mat4 get_model(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rotation, glm::mat4 mat = glm::mat4(1));
}