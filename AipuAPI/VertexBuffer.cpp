#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size): rendererId(0)
{
    GLCall(glGenBuffers(1, &rendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &rendererId));
}

void VertexBuffer::Delete() const
{
    GLCall(glDeleteBuffers(1, &rendererId));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::BufferSubData(unsigned int size, const void* data) const
{
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}
