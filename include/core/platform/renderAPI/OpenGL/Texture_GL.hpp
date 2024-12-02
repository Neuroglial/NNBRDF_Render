#pragma once
#include "core/platform/renderAPI/Texture.hpp"
#include <glad/glad.h>

namespace Texture
{
    class Texture2D_GL : public Texture2D
    {
    public:
        Texture2D_GL(Warpping_Mode wpm, Filtering_Mode ftm, bool mipmaps) : Texture2D(wpm, ftm, mipmaps), m_id(0)
        {
        }

        virtual Texture2D &operator=(Ref<Image> image) override
        {

            if (m_id)
                glDeleteTextures(1, &m_id);

            glGenTextures(1, &m_id);
            glBindTexture(GL_TEXTURE_2D, m_id);

            switch (m_wpm)
            {
            case Warpping_Mode::REPEAT:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;

            case Warpping_Mode::CLAMP_TO_EDGE:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                break;

            case Warpping_Mode::CLAMP_TO_BORDER:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &m_border_color.x);
                break;

            default:
                break;
            }

            if (m_mipmap == true)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                if (m_ftm == Filtering_Mode::LINEAR)
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }
                else
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }
            }
            else
            {
                if (m_ftm == Filtering_Mode::LINEAR)
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }
                else
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }
            }

            if(!image->m_data)
                throw std::runtime_error("Image Named " + image->m_path + " Don't Exist");

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

            if(m_mipmap)
                glGenerateMipmap(GL_TEXTURE_2D);
            
            return *this;
        }

    private:
        unsigned int m_id;
    };

} // namespace Texture2D