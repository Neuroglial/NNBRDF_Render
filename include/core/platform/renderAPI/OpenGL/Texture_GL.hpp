// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include "core/platform/renderAPI/Texture.hpp"

class Texture2D_GL : public Texture2D
{
public:
    Texture2D_GL();

    virtual void resize(int width, int height) override;

    virtual void set_sample(Tex::WarppingMode wpm, Tex::FilteringMode ftm) override;

    virtual Texture2D &operator=(Ref<Image> image) override;

    virtual void set_image(Ref<Image> image) override;

    virtual void gen_mipmap() override;

    unsigned int get_id()
    {
        return m_id;
    }

    ~Texture2D_GL();

private:
    unsigned int m_id = 0;
};

class TextureCube_GL : public TextureCube
{
public:
    TextureCube_GL();
    ~TextureCube_GL();

    virtual void set_sample(Tex::WarppingMode wpm, Tex::FilteringMode ftm) override;

    virtual void set_image(int index, Ref<Image> image) override;

    virtual void gen_mipmap() override;

    unsigned int get_id()
    {
        return m_id;
    }

private:
    unsigned int m_id = 0;
};
