#ifndef TEXTURE_H
#define TEXTURE_H

#include "Renderer.h"

class Texture
{
public:
	Texture(GLint internalFormat, int width,
		int height, unsigned char* buff);
	Texture();
	~Texture();
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;	
	void LoadBufferInTexture(unsigned char* imgBuffer, int width, int height);	
	inline unsigned int GetId() const { return rendererId; }
private:
	unsigned int rendererId;		
};

#endif // !TEXTURE_H

