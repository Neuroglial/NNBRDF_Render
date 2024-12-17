// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/platform/renderAPI/OpenGL/FrameBuffer_GL.hpp"
#include <glad/glad.h>
#include "core/platform/renderAPI/OpenGL/Texture_GL.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"

REGISTER_API(FrameBuffer_GL)

void check_framebuffer();

FrameBuffer_GL::FrameBuffer_GL()
{
    glGenFramebuffers(1, &m_id); // 生成帧缓冲对象
}

void FrameBuffer_GL::resize(int width, int height)
{
    if (m_width == width && m_height == height)
        return;

    m_width = width;
    m_height = height;

    for (auto &i : m_attachs_2D)
    {
        i.second->resize(width, height);
    }

    for (auto &i : m_attachs_cube)
    {
        i.second->resize(width, height);
    }

    if (m_depth_2D != nullptr)
        m_depth_2D->resize(width, height);

    if (m_depth_cube != nullptr)
        m_depth_cube->resize(width, height);

    if (m_renderbuffer_id)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height); // 创建深度+模板缓冲
    }
}

void FrameBuffer_GL::attach(Ref<TextureCube> &tex, int index)
{
    if (m_width == -1)
    {
        m_width = tex->get_size().x;
        m_height = tex->get_size().y;
    }

    if (m_width != tex->get_size().x || m_height != tex->get_size().y)
    {
        tex->resize(m_width, m_height);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_id); // 绑定帧缓冲对象

    if ((tex->get_channels() & Tex::Special_Mask) == Tex::Color)
    {
        auto i = m_attachs_cube.find(index);
        if (i == m_attachs_cube.end())
        {
            m_attachs_cube.emplace(index, tex);
        }
        else
        {
            m_attachs_cube[index] = tex;
        }

        if (index < 0 || index > 31)
            throw std::runtime_error("Attachment Index Error");
        if (auto tex_gl = dynamic_cast<TextureCube_GL *>(tex.get()))
        {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (unsigned int)index, tex_gl->get_id(), 0);
        }
        else
        {
            throw std::runtime_error("Texture Type Not OpenGL Error");
        }
    }
    else if ((tex->get_channels() & Tex::Special_Mask) == Tex::Depth)
    {
        m_depth_cube = tex;

        if (auto tex_gl = dynamic_cast<TextureCube_GL *>(tex.get()))
        {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex_gl->get_id(), 0);
        }
        else
        {
            throw std::runtime_error("Texture Type Not OpenGL Error");
        }
    }

    unbind();
}

void FrameBuffer_GL::attach(Ref<Texture2D> &tex, int index)
{
    if (m_width == -1)
    {
        m_width = tex->get_size().x;
        m_height = tex->get_size().y;
    }

    if (m_width != tex->get_size().x || m_height != tex->get_size().y)
    {
        tex->resize(m_width, m_height);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_id); // 绑定帧缓冲对象

    if ((tex->get_channels() & Tex::Special_Mask) == Tex::Color)
    {
        auto i = m_attachs_2D.find(index);
        if (i == m_attachs_2D.end())
        {
            m_attachs_2D.emplace(index, tex);
        }
        else
        {
            m_attachs_2D[index] = tex;
        }

        if (index < 0 || index > 31)
            throw std::runtime_error("Attachment Index Error");
        if (auto tex_gl = dynamic_cast<Texture2D_GL *>(tex.get()))
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (unsigned int)index, GL_TEXTURE_2D, tex_gl->get_id(), 0);
        }
        else
        {
            throw std::runtime_error("Texture Type Not OpenGL Error");
        }
    }
    else if ((tex->get_channels() & Tex::Special_Mask) == Tex::Depth)
    {
        m_depth_2D = tex;

        if (auto tex_gl = dynamic_cast<Texture2D_GL *>(tex.get()))
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_gl->get_id(), 0);
        }
        else
        {
            throw std::runtime_error("Texture Type Not OpenGL Error");
        }
    }

    unbind();
}

void FrameBuffer_GL::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id); // 绑定帧缓冲对象

    if (!m_renderbuffer_id && m_depth_2D == nullptr && m_depth_cube == nullptr)
    {
        glGenRenderbuffers(1, &m_renderbuffer_id); // 生成渲染缓冲对象
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height); // 创建深度+模板缓冲

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer_id);
    }

    if (!m_complete)
    {
        if (m_attachs_2D.size() == 0)
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        check_framebuffer();

        m_complete = true;
    }
}

void FrameBuffer_GL::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer_GL::~FrameBuffer_GL()
{
    unbind();
    glDeleteFramebuffers(1, &m_id);

    if (m_renderbuffer_id)
    {
        glDeleteRenderbuffers(1, &m_renderbuffer_id);
    }
}

void check_framebuffer()
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
        return;
    std::cout << "FrameBuffer Bind Error:" << std::endl;

    switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
    case GL_FRAMEBUFFER_UNDEFINED:
        std::cout << "当前绑定的帧缓冲是默认帧缓冲，且没有附加任何东西" << std::endl;
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        std::cout << "帧缓冲的一个或多个附件（如纹理或渲染缓冲）不完整或未正确配置" << std::endl;
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        std::cout << "帧缓冲没有任何附件（既没有纹理也没有渲染缓冲）" << std::endl;
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        std::cout << "指定的绘制缓冲无效或未正确配置" << std::endl;
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        std::cout << "指定的读取缓冲无效或未正确配置" << std::endl;
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        std::cout << "帧缓冲配置不被当前硬件支持（例如不兼容的格式或内部存储组合）" << std::endl;
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        std::cout << "多重采样相关设置冲突，例如样本数不一致" << std::endl;
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        std::cout << "帧缓冲的多个层级目标不一致，或者没有为立方体贴图提供完整的六个面" << std::endl;
        break;

    default:
        break;
    }

    throw std::runtime_error("Framebuffer Not Complete");
}