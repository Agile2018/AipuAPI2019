#pragma once
#pragma warning(disable : 4312)
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
	void Delete() const;
private:
	unsigned int rendererId;
};

#endif // !VERTEXARRAY_H

