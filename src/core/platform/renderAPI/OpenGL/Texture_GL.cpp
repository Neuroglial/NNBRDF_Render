// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/platform/renderAPI/OpenGL/Texture_GL.hpp"
#include <glad/glad.h>
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "core/platform/renderAPI/OpenGL/CheckTool.hpp"

REGISTER_API(Texture2D_GL)
REGISTER_API(TextureCube_GL)

uint32_t Tex_trans(uint32_t channels);

Texture2D_GL::Texture2D_GL() : Texture2D()
{
    glGenTextures(1, &m_id);
}

void Texture2D_GL::set_sample(Tex::WarppingMode wpm, Tex::FilteringMode ftm)
{
    m_wpm = wpm;
    m_ftm = ftm;

    glBindTexture(GL_TEXTURE_2D, m_id);

    switch (m_wpm)
    {
    case Tex::WarppingMode::REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;

    case Tex::WarppingMode::CLAMP:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;

    default:
        break;
    }

    switch (m_ftm)
    {
    case Tex::FilteringMode::LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case Tex::FilteringMode::NEAREST:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case Tex::FilteringMode::Mipmap:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;

    default:
        break;
    }
}

void Texture2D_GL::resize(int width, int height)
{
    m_height = height;
    m_width = width;

    glBindTexture(GL_TEXTURE_2D, m_id);

    if ((m_channels & Tex::Special_Mask) == Tex::Depth)
        glTexImage2D(GL_TEXTURE_2D, 0, Tex_trans(m_channels), m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, Tex_trans(m_channels), m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);

    GL_Check()
}

void Texture2D_GL::gen_mipmap()
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    if (m_ftm == Tex::FilteringMode::Mipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

Texture2D &Texture2D_GL::operator=(Ref<Image> image)
{
    set_image(image);
    return *this;
}

void Texture2D_GL::set_image(Ref<Image> image)
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    if (!image->m_data)
        throw std::runtime_error("Image Named " + image->m_path + " Don't Exist");

    m_path = image->m_path;

    unsigned int src_type = (image->m_channels & Tex::Type_Mask) == Tex::F ? GL_FLOAT : GL_UNSIGNED_BYTE;

    if (m_channels == Tex::None)
    {
        m_channels = image->m_channels & (Tex::CL_MASK | Tex::Bit_MASK);
    }

    m_width = image->m_width;
    
    m_height = image->m_height;

    glTexImage2D(GL_TEXTURE_2D, 0, Tex_trans(m_channels), image->m_width, image->m_height, 0, Tex_trans(image->m_channels & Tex::CL_MASK), src_type, image->m_data);

    GL_Check()

    set_sample(m_wpm, m_ftm);

}

Texture2D_GL::~Texture2D_GL()
{
    glDeleteTextures(1, &m_id);
    m_id = -1;
}

TextureCube_GL::TextureCube_GL() : TextureCube()
{
    glGenTextures(1, &m_id);
}

TextureCube_GL::~TextureCube_GL()
{
    glDeleteTextures(1, &m_id);
}

void TextureCube_GL::resize(int width, int height)
{
    m_height = height;
    m_width = width;

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

    for (int i = 0; i < 6; ++i)
    {
        if ((m_channels & Tex::Special_Mask) == Tex::Depth)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Tex_trans(m_channels), m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        else
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Tex_trans(m_channels), m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);
    }

    GL_Check()
}

void TextureCube_GL::set_sample(Tex::WarppingMode wpm, Tex::FilteringMode ftm)
{
    m_wpm = wpm;
    m_ftm = ftm;

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

    switch (m_wpm)
    {
    case Tex::WarppingMode::REPEAT:
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
        break;

    case Tex::WarppingMode::CLAMP:
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        break;

    default:
        break;
    }

    switch (m_ftm)
    {
    case Tex::FilteringMode::LINEAR:
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case Tex::FilteringMode::NEAREST:
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case Tex::FilteringMode::Mipmap:
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;

    default:
        break;
    }

    GL_Check()
}

void TextureCube_GL::gen_mipmap()
{
    if (m_ftm == Tex::FilteringMode::Mipmap)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

void TextureCube_GL::set_subImage(int index, Ref<Image> image)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

    if (!image->m_data)
        throw std::runtime_error("Image Named " + image->m_path + " Don't Exist");

    unsigned int src_type = (image->m_channels & Tex::Type_Mask) == Tex::F ? GL_FLOAT : GL_UNSIGNED_BYTE;

    if (m_channels == Tex::None)
    {
        m_channels = image->m_channels & (Tex::CL_MASK | Tex::Bit_MASK);
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, Tex_trans(m_channels), image->m_width, image->m_height, 0, Tex_trans(image->m_channels & Tex::CL_MASK), src_type, image->m_data);

    if (index == 5)
        set_sample(m_wpm, m_ftm);
}

uint32_t Tex_trans(uint32_t channels)
{
    switch (channels & Tex::Special_Mask)
    {
    case Tex::Depth:
    {
        return GL_DEPTH_COMPONENT;
    }
    default:
        break;
    }

    switch (channels & Tex::CL_MASK)
    {
    case Tex::R:
    {
        switch (channels & Tex::Bit_MASK)
        {
        case Tex::Bit8:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_RED;
            }
            case Tex::I:
            {
                return GL_R8I;
            }
            case Tex::UI:
            {
                return GL_R8UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }
        case Tex::Bit16:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_R16F;
            }
            case Tex::I:
            {
                return GL_R16I;
            }
            case Tex::UI:
            {
                return GL_R16UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }
        case Tex::Bit32:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_R32F;
            }
            case Tex::I:
            {
                return GL_R32I;
            }
            case Tex::UI:
            {
                return GL_R32UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }

        default:
            throw std::runtime_error("Error");
        }
    }
    case Tex::RG:
    {
        switch (channels & Tex::Bit_MASK)
        {
        case Tex::Bit8:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_RG;
            }
            case Tex::I:
            {
                return GL_RG8I;
            }
            case Tex::UI:
            {
                return GL_RG8UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }
        case Tex::Bit16:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_RG16F;
            }
            case Tex::I:
            {
                return GL_RG16I;
            }
            case Tex::UI:
            {
                return GL_RG16UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }
        case Tex::Bit32:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_RG32F;
            }
            case Tex::I:
            {
                return GL_RG32I;
            }
            case Tex::UI:
            {
                return GL_RG32UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }

        default:
            throw std::runtime_error("Error");
        }
    }
    case Tex::RGB:
    {
        switch (channels & Tex::Bit_MASK)
        {
        case Tex::Bit8:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_RGB;
            }
            case Tex::I:
            {
                return GL_RGB8I;
            }
            case Tex::UI:
            {
                return GL_RGB8UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }
        case Tex::Bit16:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_RGB16F;
            }
            case Tex::I:
            {
                return GL_RGB16I;
            }
            case Tex::UI:
            {
                return GL_RGB16UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }
        case Tex::Bit32:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_RGB32F;
            }
            case Tex::I:
            {
                return GL_RGB32I;
            }
            case Tex::UI:
            {
                return GL_RGB32UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }

        default:
            throw std::runtime_error("Error");
        }
    }
    case Tex::RGBA:
    {
        switch (channels & Tex::Bit_MASK)
        {
        case Tex::Bit8:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_RGBA;
            }
            case Tex::I:
            {
                return GL_RGBA8I;
            }
            case Tex::UI:
            {
                return GL_RGBA8UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }
        case Tex::Bit16:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_RGBA16F;
            }
            case Tex::I:
            {
                return GL_RGBA16I;
            }
            case Tex::UI:
            {
                return GL_RGBA16UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }
        case Tex::Bit32:
        {
            switch (channels & Tex::Type_Mask)
            {
            case Tex::F:
            {
                return GL_RGBA32F;
            }
            case Tex::I:
            {
                return GL_RGBA32I;
            }
            case Tex::UI:
            {
                return GL_RGBA32UI;
            }
            default:
                throw std::runtime_error("Error");
            }
        }

        default:
            throw std::runtime_error("Error");
        }
    }

    default:
        throw std::runtime_error("Error");
    }
}