#include "index_buffer.h"
#include "renderer.h"

index_buffer::index_buffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint))
    GL_CALL(glGenBuffers(1, &m_RendererID))
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID))
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW))
}

index_buffer::~index_buffer()
{
    GL_CALL(glDeleteBuffers(1, &m_RendererID))
}

void index_buffer::bind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID))
}

void index_buffer::unbind()
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
}

unsigned index_buffer::get_count() const
{
    return m_Count;
}
