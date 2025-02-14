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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <map>

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

Image::~Image()
{
    if (m_data != nullptr)
        stbi_image_free(m_data);
}

namespace utils
{
    glm::mat4 get_rotation(const glm::quat &q)
    {
        return glm::mat4_cast(q);
    }

    glm::vec3 to_euler(const glm::quat &q)
    {
        glm::vec3 angles;

        // Use ZYX order
        float sinp = 2 * (q.w * q.y - q.z * q.x);

        angles.y = std::asin(sinp);

        angles.z = std::atan2(2 * (q.w * q.z + q.x * q.y),
                              1 - 2 * (q.y * q.y + q.z * q.z));

        angles.x = std::atan2(2 * (q.w * q.x + q.y * q.z),
                              1 - 2 * (q.x * q.x + q.y * q.y));

        return glm::degrees(angles);
    }

    glm::quat to_quat(const glm::vec3 &degrees)
    {
        glm::vec3 radians = glm::radians(degrees);
        return glm::quat(radians); // Default ZYX order
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

    std::string read_from_file_with_include(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + path);
        }

        static std::map<std::string, std::string> file_map;

        const static std::string include = "#include";

        std::stringstream code;
        std::string line;
        while (std::getline(file, line))
        {
            if (line.find(include) == 0)
            {
                std::string includePath = line.substr(include.length() + 1);
                includePath.erase(remove(includePath.begin(), includePath.end(), '"'), includePath.end());
                auto i = file_map.find(includePath);
                if (i != file_map.end())
                {
                    code << i->second;
                }
                else
                {
                    code << read_from_file_with_include(Root_Path + includePath);
                }
            }
            else
            {
                code << line << "\n";
            }
        }

        auto i = file_map.emplace(path, code.str());
        return i.first->second;
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

    glm::mat4 get_model(const glm::vec3 &pos, const glm::vec3 &scale, const glm::quat &rotation, glm::mat4 mat)
    {
        mat = glm::translate(mat, pos);
        mat = mat * get_rotation(rotation);
        mat = glm::scale(mat, scale);

        return mat;
    }

    glm::vec3 get_position(const glm::mat4 &model)
    {
        return glm::vec3(model[3][0], model[3][1], model[3][2]);
    }

    glm::vec3 get_scale(const glm::mat4 &model)
    {
        glm::vec3 scale;
        scale.x = glm::length(glm::vec3(model[0][0], model[0][1], model[0][2]));
        scale.y = glm::length(glm::vec3(model[1][0], model[1][1], model[1][2]));
        scale.z = glm::length(glm::vec3(model[2][0], model[2][1], model[2][2]));

        return scale;
    }

    glm::quat get_rotation(const glm::mat4 &model)
    {
        return glm::quat(glm::mat3(model));
    }

    glm::vec3 get_right(const glm::mat4 &model)
    {
        auto right = glm::vec4(1, 0, 0, 0);
        right = model * right;
        right[3] = 0;
        glm::normalize(right);
        return right;
    }

    glm::vec3 get_forword(const glm::mat4 &model)
    {
        auto forword = glm::vec4(0, 0, -1, 0);
        forword = model * forword;
        forword[3] = 0;
        glm::normalize(forword);
        return forword;
    }
}