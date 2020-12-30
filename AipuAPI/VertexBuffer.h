#pragma once
#ifndef VERTEXGUFFER_H
#define VERTEXGUFFER_H

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
	void Delete() const;
	void BufferSubData(unsigned int size, const void* data) const;
private:
	unsigned int rendererId;
};

#endif // !VERTEXGUFFER_H

