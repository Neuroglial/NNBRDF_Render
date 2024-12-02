#include "utils/utils.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>


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

Ref<Image> read_image(const std::string& path,bool flip_vertically){
    Ref<Image> img = std::make_shared<Image>();

    stbi_set_flip_vertically_on_load(flip_vertically);

    img->m_data = stbi_load(path.c_str(),&img->m_width,&img->m_height,&img->m_channels,0);

    if(!img->m_data)
        throw std::runtime_error("Image Read Error");
    
    return img;
}


Image::~Image()
{
    if (m_data != nullptr)
        stbi_image_free(m_data);
}