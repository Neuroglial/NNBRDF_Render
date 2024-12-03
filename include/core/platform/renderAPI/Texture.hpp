#pragma once

#include "utils/utils.hpp"
#include <string>

namespace Texture
{
    enum Texture_Param{
        None,
        Float1,
        Float2,
        Float3,
        Float4
    };

    enum Warpping_Mode{
        REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
    };

    enum Filtering_Mode{
        NEAREST,
        LINEAR,
    };

    


    
    class Texture2D
    {
    protected:
        Warpping_Mode m_wpm;
        Filtering_Mode m_ftm;
        Texture_Param m_type;
        int m_width,m_height;
        bool m_mipmap;
        std::string m_path;
        glm::vec3 m_border_color;

    public:
        Texture2D(Warpping_Mode wpm = REPEAT,Filtering_Mode ftm = LINEAR,bool mipmap = true):
            m_wpm(wpm),m_ftm(ftm),m_type(Texture_Param::None),m_mipmap(mipmap),m_border_color(glm::vec3(0.0f)){
        }

        Texture2D() = delete;

        const std::string& get_path(){
            return m_path;
        }

        virtual Texture2D& operator=(Ref<Image> image) = 0; 

        virtual Texture2D& set_image(Ref<Image> image) = 0; 

        virtual ~Texture2D() {};
    };
    



} // namespace Texture
