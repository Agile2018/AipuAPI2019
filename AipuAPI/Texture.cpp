#include "Texture.h"

Texture::Texture(GLint internalFormat, int width, int height, unsigned char* buff)
{	
	GLCall(glGenTextures(1, &rendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, rendererId));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); //GL_REPEAT
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height,
		0, internalFormat, GL_UNSIGNED_BYTE, buff));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::Texture() : rendererId(0)
{	
	GLCall(glGenTextures(1, &rendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, rendererId));
	// Set texture filtering
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); //GL_REPEAT
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &rendererId));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, rendererId));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::LoadBufferInTexture(unsigned char* imgBuffer, int width, int height)
{
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, imgBuffer));
}
