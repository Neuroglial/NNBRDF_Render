#include "core/platform/renderAPI/OpenGL/FrameBuffer_GL.hpp"
#include <glad/glad.h>
#include "core/platform/renderAPI/OpenGL/Texture_GL.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"

REGISTER_API(FrameBuffer_GL)

void FrameBuffer_GL::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    for (auto &i : m_attachs)
    {
        i.second->resize(width, height);
    }

    if (m_renderbuffer_id)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height); // 创建深度+模板缓冲
    }
}

void FrameBuffer_GL::attach(Ref<Texture2D> &tex, AttachType type, int index)
{
    auto i = m_attachs.find(index);
    if (i == m_attachs.end())
    {
        m_attachs.emplace(index,tex);
    }
    else
    {
        m_attachs[index] = tex;
    }

    if (m_width == -1)
    {
        m_width = tex->get_size().x;
        m_height = tex->get_size().y;
    }

    if (m_width != tex->get_size().x || m_height != tex->get_size().y)
    {
        tex->resize(m_width, m_height);
    }

    if (!m_id)
        glGenFramebuffers(1, &m_id);         // 生成帧缓冲对象
    glBindFramebuffer(GL_FRAMEBUFFER, m_id); // 绑定帧缓冲对象

    if (type == Color)
    {
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
    else if (type == DepthStencil)
    {
        m_depth_attach = true;
    }

    unbind();
}

void FrameBuffer_GL::clear()
{
}

void FrameBuffer_GL::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id); // 绑定帧缓冲对象
    if (!m_renderbuffer_id && !m_depth_attach)
    {
        glGenRenderbuffers(1, &m_renderbuffer_id); // 生成渲染缓冲对象
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height); // 创建深度+模板缓冲

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer_id);
    }

    if (!m_complete)
    {
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Framebuffer Not Complete");
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
    if (m_id)
    {
        glDeleteFramebuffers(1, &m_id);
    }

    if (m_renderbuffer_id)
    {
        glDeleteRenderbuffers(1, &m_renderbuffer_id);
    }
}