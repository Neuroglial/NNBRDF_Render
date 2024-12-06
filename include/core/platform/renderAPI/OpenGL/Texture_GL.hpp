#pragma once
#include "core/platform/renderAPI/Texture.hpp"

namespace Texture
{
    class Texture2D_GL : public Texture2D
    {
    public:
        Texture2D_GL(Warpping_Mode wpm, Filtering_Mode ftm) : Texture2D(wpm, ftm), m_id(0){
        }

        Texture2D_GL(int width, int height, Texture_Param param = RGB, Warpping_Mode wpm = REPEAT, Filtering_Mode ftm = LINEAR);

        Texture2D_GL(Warpping_Mode wpm, Filtering_Mode ftm, Ref<Image> image) : Texture2D(wpm, ftm), m_id(0){
            set_image(image);
        }

        Texture2D_GL() = delete;

        virtual Texture2D &operator=(Ref<Image> image) override;

        virtual Texture2D &set_image(Ref<Image> image) override;

        virtual void gen_mipmap() override;

        unsigned int get_id(){return m_id;}

        ~Texture2D_GL();

    private:

        unsigned int m_id;
    };

} // namespace Texture2D