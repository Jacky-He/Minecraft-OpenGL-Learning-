#include "IndexBuffer.hpp"
#include "Util.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, int mode):m_Count(count)
{
    assert (sizeof(unsigned int) == sizeof(GLuint));
    
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, mode));
}

void IndexBuffer::SetData(const unsigned int* data, unsigned int count) const
{
    Bind();
    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count*sizeof(unsigned int), data));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
