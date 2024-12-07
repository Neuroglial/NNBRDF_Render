#include "core/platform/renderAPI/OpenGL/Texture_GL.hpp"

#include <glad/glad.h>

Texture::Texture2D_GL::Texture2D_GL(int width, int height, Texture_Param param, Warpping_Mode wpm, Filtering_Mode ftm) : Texture2D(width, height, param, wpm, ftm), m_id(0)
{
    resize(width, height);
    reset_sample(wpm, ftm);
}

void Texture::Texture2D_GL::reset_sample(Warpping_Mode wpm, Filtering_Mode ftm)
{
    m_wpm = wpm;
    m_ftm = ftm;

    glBindTexture(GL_TEXTURE_2D, m_id);

    switch (m_wpm)
    {
    case Warpping_Mode::REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;

    case Warpping_Mode::CLAMP:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;

    default:
        break;
    }

    switch (m_ftm)
    {
    case Filtering_Mode::LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case Filtering_Mode::NEAREST:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case Filtering_Mode::Mipmap:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;

    default:
        break;
    }
}

void Texture::Texture2D_GL::resize(int width, int height)
{
    m_height = height;
    m_width = width;

    if (!m_id)
        glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexImage2D(GL_TEXTURE_2D, 0, m_type, m_width, m_height, 0, m_type, GL_UNSIGNED_BYTE, NULL);
}

void Texture::Texture2D_GL::gen_mipmap()
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    if (m_ftm == Texture::Mipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

Texture::Texture2D &Texture::Texture2D_GL::operator=(Ref<Image> image)
{
    set_image(image);
    return *this;
}

Texture::Texture2D &Texture::Texture2D_GL::set_image(Ref<Image> image)
{

    if (!m_id)
        glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);

    if (!image->m_data)
        throw std::runtime_error("Image Named " + image->m_path + " Don't Exist");

    m_path = image->m_path;

    if (image->m_channels == 1)
    {
        m_type = Texture_Param::R;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R, image->m_width, image->m_height, 0, GL_R, GL_UNSIGNED_BYTE, image->m_data);
    }
    else if (image->m_channels == 2)
    {
        m_type = Texture_Param::RG;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, image->m_width, image->m_height, 0, GL_RG, GL_UNSIGNED_BYTE, image->m_data);
    }
    else if (image->m_channels == 3)
    {
        m_type = Texture_Param::RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->m_width, image->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->m_data);
    }
    else if (image->m_channels == 4)
    {
        m_type = Texture_Param::RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->m_width, image->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->m_data);
    }

    reset_sample(m_wpm, m_ftm);

    gen_mipmap();

    return *this;
}

Texture::Texture2D_GL::~Texture2D_GL()
{
    if (m_id)
        glDeleteTextures(1, &m_id);
}