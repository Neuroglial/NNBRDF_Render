#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

enum RendererType{
    None,
    OpenGL,
};

struct Image{
    int m_width;
    int m_height;
    unsigned char* m_data;
    int m_channels;

    std::string m_path;

    Image():m_width(0),m_height(0),m_data(nullptr),m_channels(0){}

    ~Image();
};

namespace std
{
    string to_string(const glm::vec2 &val);
    string to_string(const glm::vec3 &val);
    string to_string(const glm::vec4 &val);
}

std::string read_from_file(const std::string& path);

Ref<Image> read_image(const std::string& path,bool flip_vertically=true);