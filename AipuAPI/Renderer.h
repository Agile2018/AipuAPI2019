#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderGL.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	Renderer();
	~Renderer();
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const ShaderGL& shader) const;
	void DrawRectangle(const VertexArray& va, const IndexBuffer& ib, const ShaderGL& shader) const;
	void SetLineWidth(GLfloat value) {
		lineWidth = value;
	}
	void DrawText(const VertexArray& va, const VertexBuffer& vb,
		const ShaderGL& shader, unsigned int size, const void* data) const;
private:
	GLfloat lineWidth = 3.0f;
};


#endif // !RENDERER_H

