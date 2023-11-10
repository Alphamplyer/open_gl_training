#include "vertex_buffer.h"
#include "renderer.h"

vertex_buffer::vertex_buffer(const void* data, unsigned size)
{
    GL_CALL(glGenBuffers(1, &m_RendererID))
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID))
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW))
}

vertex_buffer::~vertex_buffer()
{
    GL_CALL(glDeleteBuffers(1, &m_RendererID))
}

void vertex_buffer::bind() const
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID))
}

void vertex_buffer::unbind()
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0))
}