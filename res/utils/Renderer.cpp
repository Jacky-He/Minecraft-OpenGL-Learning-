#include "Renderer.hpp"
#include "Util.hpp"

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader, int count) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawLines(const VertexArray &va, const Shader& shader, int count) const
{
    shader.Bind();
    va.Bind();
    GLCall(glEnable(GL_LINE_SMOOTH));
//    GLCall(glLineWidth(2.0f));
    GLCall(glDrawArrays(GL_LINES, 0, count));
    GLCall(glDisable(GL_LINE_SMOOTH));
}
