#include "core/platform/renderAPI/OpenGL/Texture_GL.hpp"

#include <glad/glad.h>


Texture::Texture2D &Texture::Texture2D_GL::operator=(Ref<Image> image)
{
    set_image(image);
    return *this;
}

Texture::Texture2D &Texture::Texture2D_GL::set_image(Ref<Image> image)
{

    if (m_id)
        glDeleteTextures(1, &m_id);

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    if (!image->m_data)
        throw std::runtime_error("Image Named " + image->m_path + " Don't Exist");

    m_path = image->m_path;

    if (image->m_channels == 1)
    {
        m_type = Texture_Param::Float1;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R, image->m_width, image->m_height, 0, GL_R, GL_UNSIGNED_BYTE, image->m_data);
    }
    else if (image->m_channels == 2)
    {
        m_type = Texture_Param::Float2;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, image->m_width, image->m_height, 0, GL_RG, GL_UNSIGNED_BYTE, image->m_data);
    }
    else if (image->m_channels == 3)
    {
        m_type = Texture_Param::Float3;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->m_width, image->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->m_data);
    }
    else if (image->m_channels == 4)
    {
        m_type = Texture_Param::Float4;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->m_width, image->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->m_data);
    }

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
        glGenerateMipmap(GL_TEXTURE_2D);
        break;
    
    default:
        break;
    }
    
    return *this;
}

void Texture::Texture2D_GL::set_border_color(const glm::vec3& border_color){
    m_border_color = border_color;
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &m_border_color.x);
}

Texture::Texture2D_GL::~Texture2D_GL()
{
    if (m_id)
        glDeleteTextures(1, &m_id);
}