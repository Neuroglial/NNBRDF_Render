#include "core/platform/renderAPI/OpenGL/Texture_GL.hpp"
#include <glad/glad.h>
#include "core/platform/renderAPI/RenderAPI.hpp"

REGISTER_API(Texture2D_GL)

void Texture2D_GL::set_sample(Tex_WarppingMode wpm, Tex_FilteringMode ftm)
{
    m_wpm = wpm;
    m_ftm = ftm;

    if (!m_id)
        glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);

    switch (m_wpm)
    {
    case Tex_WarppingMode::REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;

    case Tex_WarppingMode::CLAMP:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;

    default:
        break;
    }

    switch (m_ftm)
    {
    case Tex_FilteringMode::LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case Tex_FilteringMode::NEAREST:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case Tex_FilteringMode::Mipmap:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;

    default:
        break;
    }

    gen_mipmap();
}

void Texture2D_GL::resize(int width, int height)
{
    m_height = height;
    m_width = width;

    if (!m_id)
        glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexImage2D(GL_TEXTURE_2D, 0, (int)m_channels, m_width, m_height, 0, (int)m_channels, GL_UNSIGNED_BYTE, NULL);
}

void Texture2D_GL::gen_mipmap()
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    if (m_ftm == Tex_FilteringMode::Mipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

Texture2D &Texture2D_GL::operator=(Ref<Image> image)
{
    set_image(image);
    return *this;
}

Texture2D &Texture2D_GL::set_image(Ref<Image> image)
{

    if (!m_id)
        glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);

    if (!image->m_data)
        throw std::runtime_error("Image Named " + image->m_path + " Don't Exist");

    m_path = image->m_path;

    if (image->m_channels == 1)
    {
        m_channels = Tex_Channels::R;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R, image->m_width, image->m_height, 0, GL_R, GL_UNSIGNED_BYTE, image->m_data);
    }
    else if (image->m_channels == 2)
    {
        m_channels = Tex_Channels::RG;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, image->m_width, image->m_height, 0, GL_RG, GL_UNSIGNED_BYTE, image->m_data);
    }
    else if (image->m_channels == 3)
    {
        m_channels = Tex_Channels::RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->m_width, image->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->m_data);
    }
    else if (image->m_channels == 4)
    {
        m_channels = Tex_Channels::RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->m_width, image->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->m_data);
    }

    set_sample(m_wpm, m_ftm);

    return *this;
}

Texture2D_GL::~Texture2D_GL()
{
    if (m_id)
        glDeleteTextures(1, &m_id);
}