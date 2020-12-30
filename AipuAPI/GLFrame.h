#ifndef GLFRAME_H
#define GLFRAME_H
#include <iostream>
#include <map>
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ShaderGL.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "FrameData.h"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class GLFrame
{
public:
	GLFrame();
	~GLFrame();
	void SetOrthographicProjection(float frameWidth, float frameHeight);	
	void InitFrame();
	void DrawFrame();
	void SetFlowImage(unsigned char* imgBuffer, int width, int height);
	void Viewport(GLint x, GLint y, GLsizei screenWidth, GLsizei screenHeight);				

	inline void SetLastTime(double value) { lastTime = value; }
	inline double GetLastTime() { return lastTime; }
	inline void SetSlot(unsigned int value) { slot = value; }
	
	inline void SetLabel(std::string value) { label = value; }
	inline void SetFrameData(FrameData* frmDat) { frameData = frmDat; }

private:
	float windowWidth = 0.0f, windowHeight = 0.0f, 
		orthographicFrameWidth = 0.0f, orthographicFrameHeight = 0.0f, 
		xFont = 0.0f, yFont = 0.0f;
	double lastTime = 0.0;
	unsigned int slot = 0;	
	GLint xView = 0, yView = 0;
	GLushort pattern = 0xAAAA; //0x18ff 0xAAAA
	GLfloat  factor = 4.0f;
	FrameData* frameData = nullptr;
	std::string label;
	std::vector<Texture*> texturesFonts;
	std::map<GLchar, Character> Characters;
	std::vector<GLfloat> verticesTexture;
	std::vector<GLuint> indicesTexture;
	std::vector<GLfloat> verticesRectangle;
	std::vector<GLuint> indicesRectangle;	
	//BUILD TEXTURE
	IndexBuffer* indexBufferTexture = nullptr;
	VertexBuffer* vertexBufferTexture = nullptr;
	VertexArray* vertexArrayTexture = nullptr;
	VertexBufferLayout layoutTexture;
	ShaderGL* shaderGLTexture = nullptr;	
	Texture* texture = nullptr;

	//BUILD RECTANGLE
	IndexBuffer* indexBufferRectangle = nullptr;
	VertexBuffer* vertexBufferRectangle = nullptr;
	VertexArray* vertexArrayRectangle = nullptr;	
	ShaderGL* shaderGLRectangle = nullptr;
	VertexBufferLayout layoutRectangle;

	// BUILD FONTS
	VertexBuffer* vertexBufferFonts = nullptr;
	VertexArray* vertexArrayFonts = nullptr;
	VertexBufferLayout layoutFonts;
	ShaderGL* shaderGLFonts;

	Renderer renderer;
	void InitIndex();
	void InitVertices();
	void BuildTexture();
	void BuildRectangle();
	void BuildStipple();
	void InitStipple();
	void InitRectangle();
	void BuildFonts();
	void DrawFonts(std::string text, float x, float y, float scale, glm::vec3 color);
};


#endif // !GLFRAME_H
