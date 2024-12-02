#pragma once

#include <string>

namespace Texture
{
    enum Texture_Param{
        Float1,
        Float2,
        Float3,
        Float3,
    };

    enum Warpping_Mode{

    };

    enum Filtering_Mode{

    };

    


    
    class Texture2D
    {
    protected:
        Warpping_Mode m_wpm;
        Filtering_Mode m_ftm;
        int m_width,m_height;

        std::string path;


    public:
        Texture2D(Warpping_Mode wpm,Filtering_Mode ftm):m_wpm(wpm),m_ftm(ftm){
        }

        ~Texture2D();
    };
    



} // namespace Texture
