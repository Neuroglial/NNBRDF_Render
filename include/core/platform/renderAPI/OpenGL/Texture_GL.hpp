// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include "core/platform/renderAPI/Texture.hpp"

class Texture2D_GL : public Texture2D
{
public:
    Texture2D_GL(Tex_WarppingMode wpm, Tex_FilteringMode ftm) : Texture2D(wpm, ftm)
    {
        set_sample(wpm, ftm);
    }

    Texture2D_GL(int width, int height, Tex_Channels param, Tex_WarppingMode wpm, Tex_FilteringMode ftm) : Texture2D(width, height, param, wpm, ftm)
    {
        resize(width, height);
        set_sample(wpm, ftm);
    }

    Texture2D_GL(Tex_WarppingMode wpm, Tex_FilteringMode ftm, Ref<Image> image) : Texture2D(wpm, ftm, image)
    {
        set_image(image);
    }

    Texture2D_GL() : Texture2D()
    {
        m_channels = Tex_Channels::RGB;
        resize(1, 1);
        set_sample(Tex_WarppingMode::CLAMP, Tex_FilteringMode::LINEAR);
    }

    virtual void resize(int width, int height) override;

    virtual void set_sample(Tex_WarppingMode wpm, Tex_FilteringMode ftm) override;

    virtual Texture2D &operator=(Ref<Image> image) override;

    virtual Texture2D &set_image(Ref<Image> image) override;

    virtual void gen_mipmap() override;

    unsigned int get_id() { return m_id; }

    ~Texture2D_GL();

private:
    unsigned int m_id = 0;
};
