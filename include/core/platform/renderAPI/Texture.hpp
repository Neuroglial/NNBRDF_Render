#pragma once

#include "utils/utils.hpp"
#include <string>

namespace Texture
{
    enum Texture_Param: uint16_t
    {
        None,
        R =  0x2002,    //GL_R
        RG = 0x8227,     //GL_RG
        RGB = 0x1907,   //GL_RGB
        RGBA = 0x1908  //GL_RGBA
    };

    enum Warpping_Mode
    {
        REPEAT,
        CLAMP,
    };

    enum Filtering_Mode
    {
        NEAREST,
        LINEAR,
        Mipmap,
    };

    class Texture2D
    {
    protected:
        Warpping_Mode m_wpm;
        Filtering_Mode m_ftm;
        Texture_Param m_type;
        int m_width, m_height;
        std::string m_path;

    public:
        Texture2D(Warpping_Mode wpm = REPEAT, Filtering_Mode ftm = LINEAR) : m_wpm(wpm), m_ftm(ftm), m_type(Texture_Param::None)
        {
        }

        Texture2D(int width, int height, Texture_Param pm = RGB, Warpping_Mode wpm = REPEAT, Filtering_Mode ftm = LINEAR) : 
            m_width(width), m_height(height), m_wpm(wpm), m_ftm(ftm), m_type(pm)
        {
        }

        Texture2D() = delete;

        const std::string &get_path()
        {
            return m_path;
        }

        virtual Texture2D &operator=(Ref<Image> image) = 0;

        virtual Texture2D &set_image(Ref<Image> image) = 0;

        virtual void gen_mipmap() = 0;

        virtual ~Texture2D() {};
    };

} // namespace Texture
