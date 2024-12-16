#include "core/platform/renderAPI/FrameBuffer.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"

void FrameBuffer::bind(const glm::vec4 &clear_color)
{
    bind();
    RenderAPI::clear(clear_color);
}

void FrameBuffer::init(int width, int height)
{
    m_width = width;
    m_height = height;
}