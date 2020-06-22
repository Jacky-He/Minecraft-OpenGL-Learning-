#include "VertexBuffer.hpp"
#include "Util.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, int mode)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, mode));
}

void VertexBuffer::SetData(const void* data, unsigned int size) const
{
    Bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

void VertexBuffer::SetDataMap(const void* data, unsigned int size)
{
    Bind();
    void* store = glMapBuffer (GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    std::memcpy(store, data, size);
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

VertexBuffer::~VertexBuffer()
{
    Bind();
    GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

