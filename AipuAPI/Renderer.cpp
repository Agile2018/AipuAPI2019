#include "Renderer.h"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError())
    {
        std::cout << "[OPENGL Error] (" << error << "): " << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const ShaderGL& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawRectangle(const VertexArray& va, const IndexBuffer& ib, const ShaderGL& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();    
    glLineWidth(lineWidth);
    GLCall(glDrawElements(GL_LINE_STRIP, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawText(const VertexArray& va, const VertexBuffer& vb,
    const ShaderGL& shader, unsigned int size, const void* data) const
{
    shader.Bind();
    va.Bind();
    vb.Bind();
    vb.BufferSubData(size, data);
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
   
}