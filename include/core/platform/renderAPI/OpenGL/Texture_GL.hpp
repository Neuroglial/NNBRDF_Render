#pragma once
#include "core/platform/renderAPI/Texture.hpp"

class Texture2D_GL : public Texture2D
{
public:
    Texture2D_GL(Tex_WarppingMode wpm, Tex_FilteringMode ftm) : Texture2D(wpm, ftm), m_id(0)
    {
    }

    Texture2D_GL(int width, int height, Tex_Param param = Tex_Param::RGB, Tex_WarppingMode wpm = Tex_WarppingMode::REPEAT, Tex_FilteringMode ftm = Tex_FilteringMode::LINEAR);

    Texture2D_GL(Tex_WarppingMode wpm, Tex_FilteringMode ftm, Ref<Image> image) : Texture2D(wpm, ftm), m_id(0)
    {
        set_image(image);
    }

    virtual void resize(int width, int height) override;

    virtual void reset_sample(Tex_WarppingMode wpm, Tex_FilteringMode ftm) override;

    Texture2D_GL() = delete;

    virtual Texture2D &operator=(Ref<Image> image) override;

    virtual Texture2D &set_image(Ref<Image> image) override;

    virtual void gen_mipmap() override;

    unsigned int get_id() { return m_id; }

    ~Texture2D_GL();

private:
    unsigned int m_id;
};
