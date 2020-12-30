#include "GLFrame.h"

GLFrame::GLFrame()
{    
    InitVertices();
    InitIndex();
}

GLFrame::~GLFrame()
{
    verticesTexture.clear();
    indicesTexture.clear();
    indicesRectangle.clear();
    texturesFonts.clear();
    verticesRectangle.clear();
    Characters.clear();
}

void GLFrame::InitFrame()
{
    BuildTexture();
    InitStipple();
    //InitRectangle();
    BuildFonts();
}

void GLFrame::DrawFrame()
{
    /*if (vertexArrayRectangle != NULL)
    {
        vertexArrayRectangle->Delete();        
        vertexBufferRectangle->Delete();               
    }*/

    GLCall(glViewport(xView, yView, (GLsizei)windowWidth, (GLsizei)windowHeight));
    texture->Bind(slot);    
    renderer.Draw(*vertexArrayTexture, *indexBufferTexture, *shaderGLTexture);
    //BuildRectangle();
    BuildStipple();    
    DrawFonts(label.c_str(), xFont, yFont, 0.5f, glm::vec3(frameData->GetColourLabelR(), 
        frameData->GetColourLabelG(), frameData->GetColourLabelB()));
    texture->Unbind();
    vertexArrayTexture->Unbind();
    indexBufferTexture->Unbind();
    shaderGLTexture->Unbind();
    vertexArrayRectangle->Unbind();
    indexBufferRectangle->Unbind();
    shaderGLRectangle->Unbind();
}

void GLFrame::SetFlowImage(unsigned char* imgBuffer, int width, int height)
{    
    texture->Bind();
    texture->LoadBufferInTexture(imgBuffer, width, height);
    texture->Unbind();
}

void GLFrame::Viewport(GLint x, GLint y, GLsizei screenWidth, GLsizei screenHeight)
{
    windowWidth = (float)screenWidth;
    windowHeight = (float)screenHeight;
    xView = x;
    yView = y;    
    if (xFont == 0.0f)
    {
        xFont = ((float)screenWidth / 100.0f) * 3.0f;
        yFont = ((float)screenHeight / 100.0f) * 91.0f;
    }
}

void GLFrame::SetOrthographicProjection(float frameWidth, float frameHeight)
{
    orthographicFrameWidth = frameWidth;
    orthographicFrameHeight = frameHeight;
}
void GLFrame::InitIndex() {
    indicesTexture.push_back(0);
    indicesTexture.push_back(1);
    indicesTexture.push_back(3);
    indicesTexture.push_back(1);
    indicesTexture.push_back(2);
    indicesTexture.push_back(3);

    indicesRectangle.push_back(0);
    indicesRectangle.push_back(1);
    indicesRectangle.push_back(2);
    indicesRectangle.push_back(3);
    indicesRectangle.push_back(4);
    indicesRectangle.push_back(5);
    indicesRectangle.push_back(6);
    indicesRectangle.push_back(7);

}

void GLFrame::InitVertices()
{
    verticesTexture.push_back(1.0f); //
    verticesTexture.push_back(1.0f); // TOP RIGHT  
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(1.0f);
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(1.0f);
    verticesTexture.push_back(1.0f);
    verticesTexture.push_back(1.0f); //
    verticesTexture.push_back(-1.0f);//BOTTOM RIGHT 
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(1.0f);
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(1.0f);
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(-1.0f); //
    verticesTexture.push_back(-1.0f); // BOTTOM LEFT 
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(1.0f);
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(-1.0f); //
    verticesTexture.push_back(1.0f); // TOP LEFT 
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(1.0f);
    verticesTexture.push_back(1.0f);
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(0.0f);
    verticesTexture.push_back(1.0f);
}

void GLFrame::BuildTexture()
{
    vertexArrayTexture = new VertexArray();
    vertexBufferTexture = new VertexBuffer(&verticesTexture[0], 
        (unsigned int)verticesTexture.size() * sizeof(float));
    layoutTexture.Push<float>(3); // 2 3
    layoutTexture.Push<float>(3); // 2 3
    layoutTexture.Push<float>(2);
    vertexArrayTexture->AddBuffer(*vertexBufferTexture, layoutTexture);
    indexBufferTexture = new IndexBuffer(&indicesTexture[0], 6);    
    shaderGLTexture = new ShaderGL("Texture.shader");
    shaderGLTexture->Bind();
    shaderGLTexture->SetUniform1i("ourTexture", 0);
    texture = new Texture();           
    vertexArrayTexture->Unbind();
    vertexBufferTexture->Unbind();
    indexBufferTexture->Unbind();
    shaderGLTexture->Unbind();
}

void GLFrame::InitRectangle() {
    layoutRectangle.Push<float>(2);
    indexBufferRectangle = new IndexBuffer(&indicesRectangle[0], 8);
    glm::mat4 proj = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f); //
    shaderGLRectangle = new ShaderGL("Rectangle.shader");
    shaderGLRectangle->Bind();
    shaderGLRectangle->SetUniformMat4f("u_MVP", proj);
    shaderGLRectangle->SetUniform4f("u_Color", frameData->GetColourR(0),
        frameData->GetColourG(0), frameData->GetColourB(0), 1.0f);
    shaderGLRectangle->Unbind();
    indexBufferRectangle->Unbind();
}

void GLFrame::InitStipple() {
    layoutRectangle.Push<float>(2);
    indexBufferRectangle = new IndexBuffer(&indicesRectangle[0], 8);  
    glm::mat4 proj = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f); //
    shaderGLRectangle = new ShaderGL("Stipple.shader");
    shaderGLRectangle->Bind();
    shaderGLRectangle->SetUniform1f("u_factor", factor);
    shaderGLRectangle->SetUniform1ui("u_pattern", pattern);
    shaderGLRectangle->SetUniform2f("u_resolution", windowWidth, windowHeight);
    shaderGLRectangle->SetUniformMat4f("u_MVP", proj);
    shaderGLRectangle->SetUniform4f("u_Color", frameData->GetColourR(0),
        frameData->GetColourG(0), frameData->GetColourB(0), 1.0f);    
    shaderGLRectangle->Unbind();
    indexBufferRectangle->Unbind();    
}

void GLFrame::BuildFonts()
{
    vertexArrayFonts = new VertexArray();
    vertexBufferFonts = new VertexBuffer(NULL, sizeof(float) * 6 * 4);
    layoutFonts.Push<float>(4);
    vertexArrayFonts->AddBuffer(*vertexBufferFonts, layoutFonts);
    shaderGLFonts = new ShaderGL("Fonts.shader");
    shaderGLFonts->Bind();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, 
        static_cast<float>(windowHeight), -1.0f, 1.0f); 
    shaderGLFonts->SetUniformMat4f("projection", projection);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    }
    FT_Face face;
    if (FT_New_Face(ft, "arialbd.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        //86 105 100 101 111 32 49 50 51 52
        // load first 128 characters of ASCII set

        for (unsigned char c = 0; c < 128; c++)
        {
            if (c == 86 || c == 105 || c == 100 || c == 101
                || c == 111 || c == 32 || c == 49 || c == 50 || c == 51 || c == 52)
            {
                // Load character glyph 
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                Texture* textureFont = new Texture(GL_RED, face->glyph->bitmap.width,
                    face->glyph->bitmap.rows, face->glyph->bitmap.buffer);


                // now store character for later use
                Character character = {
                    textureFont->GetId(),
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Character>(c, character));
                texturesFonts.push_back(textureFont);
                textureFont->Unbind();
            }


        }
        
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    shaderGLFonts->Unbind();
    vertexArrayFonts->Unbind();
    vertexBufferFonts->Unbind();
}

void GLFrame::DrawFonts(std::string text, float x, float y, float scale, glm::vec3 color)
{
    shaderGLFonts->Bind();
    shaderGLFonts->SetUniform3f("textColor", color.x, color.y, color.z);
    shaderGLFonts->Unbind();

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float verticesText[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        unsigned int id = ch.TextureID;
        auto it = find_if(texturesFonts.begin(), texturesFonts.end(), 
            [&id](Texture* obj) {return obj->GetId() == id; });

        if (it != texturesFonts.end()) {
            int index = (int)std::distance(texturesFonts.begin(), it);

            texturesFonts[index]->Bind();
            renderer.DrawText(*vertexArrayFonts, *vertexBufferFonts,
                *shaderGLFonts, sizeof(verticesText), verticesText);
            texturesFonts[index]->Unbind();
        }

        vertexArrayFonts->Unbind();
        vertexBufferFonts->Unbind();
        x += (ch.Advance >> 6) * scale; 
    }

    shaderGLFonts->Unbind();
}

void GLFrame::BuildStipple() {
    
    for (int i = 0; i < NUM_RECTANGLES; i++) {
        if (frameData->GetRatioWidth(i) != 0.0f) {
            verticesRectangle.clear();
            verticesRectangle.push_back(frameData->GetPositionX(i) - frameData->GetRatioWidth(i)); // BOTTOM LEFT TO BOTTOM RIGHT
            verticesRectangle.push_back(frameData->GetPositionY(i) - frameData->GetRatioHeight(i));
            verticesRectangle.push_back(frameData->GetPositionX(i) + frameData->GetRatioWidth(i));
            verticesRectangle.push_back(frameData->GetPositionY(i) - frameData->GetRatioHeight(i));

            verticesRectangle.push_back(frameData->GetPositionX(i) + frameData->GetRatioWidth(i)); //BOTTOM RIGHT TO TOP RIGHT
            verticesRectangle.push_back(frameData->GetPositionY(i) - frameData->GetRatioHeight(i));
            verticesRectangle.push_back(frameData->GetPositionX(i) + frameData->GetRatioWidth(i));
            verticesRectangle.push_back(frameData->GetPositionY(i) + frameData->GetRatioHeight(i));

            verticesRectangle.push_back(frameData->GetPositionX(i) + frameData->GetRatioWidth(i)); // TOP RIGHT TO TOP LEFT
            verticesRectangle.push_back(frameData->GetPositionY(i) + frameData->GetRatioHeight(i));
            verticesRectangle.push_back(frameData->GetPositionX(i) - frameData->GetRatioWidth(i));
            verticesRectangle.push_back(frameData->GetPositionY(i) + frameData->GetRatioHeight(i));

            verticesRectangle.push_back(frameData->GetPositionX(i) - frameData->GetRatioWidth(i)); // TOP LEFT TO BOTTOM LEFT
            verticesRectangle.push_back(frameData->GetPositionY(i) + frameData->GetRatioHeight(i));
            verticesRectangle.push_back(frameData->GetPositionX(i) - frameData->GetRatioWidth(i));
            verticesRectangle.push_back(frameData->GetPositionY(i) - frameData->GetRatioHeight(i));

            vertexArrayRectangle = new VertexArray();
            vertexBufferRectangle = new VertexBuffer(&verticesRectangle[0],
                (unsigned int)verticesRectangle.size() * sizeof(float));
            vertexArrayRectangle->AddBuffer(*vertexBufferRectangle, layoutRectangle);
            shaderGLRectangle->Bind();
            shaderGLRectangle->SetUniform2f("u_resolution", windowWidth, windowHeight);
            shaderGLRectangle->SetUniform4f("u_Color", frameData->GetColourR(i), 
                frameData->GetColourG(i), frameData->GetColourB(i), 1.0f);
            renderer.DrawRectangle(*vertexArrayRectangle, *indexBufferRectangle, *shaderGLRectangle);
            vertexArrayRectangle->Unbind();
            vertexBufferRectangle->Unbind();
            shaderGLRectangle->Unbind();
            vertexArrayRectangle->Delete();
            vertexBufferRectangle->Delete();

        }
    }
}

void GLFrame::BuildRectangle()
{        
    for (int i = 0; i < NUM_RECTANGLES; i++) {
        if (frameData->GetRatioWidth(i) != 0.0f) {
            verticesRectangle.clear();
            verticesRectangle.push_back(frameData->GetPositionX(i) - frameData->GetRatioWidth(i)); // BOTTOM LEFT TO BOTTOM RIGHT
            verticesRectangle.push_back(frameData->GetPositionY(i) - frameData->GetRatioHeight(i));
            verticesRectangle.push_back(frameData->GetPositionX(i) + frameData->GetRatioWidth(i));
            verticesRectangle.push_back(frameData->GetPositionY(i) - frameData->GetRatioHeight(i));

            verticesRectangle.push_back(frameData->GetPositionX(i) + frameData->GetRatioWidth(i)); //BOTTOM RIGHT TO TOP RIGHT
            verticesRectangle.push_back(frameData->GetPositionY(i) - frameData->GetRatioHeight(i));
            verticesRectangle.push_back(frameData->GetPositionX(i) + frameData->GetRatioWidth(i));
            verticesRectangle.push_back(frameData->GetPositionY(i) + frameData->GetRatioHeight(i));

            verticesRectangle.push_back(frameData->GetPositionX(i) + frameData->GetRatioWidth(i)); // TOP RIGHT TO TOP LEFT
            verticesRectangle.push_back(frameData->GetPositionY(i) + frameData->GetRatioHeight(i));
            verticesRectangle.push_back(frameData->GetPositionX(i) - frameData->GetRatioWidth(i));
            verticesRectangle.push_back(frameData->GetPositionY(i) + frameData->GetRatioHeight(i));

            verticesRectangle.push_back(frameData->GetPositionX(i) - frameData->GetRatioWidth(i)); // TOP LEFT TO BOTTOM LEFT
            verticesRectangle.push_back(frameData->GetPositionY(i) + frameData->GetRatioHeight(i));
            verticesRectangle.push_back(frameData->GetPositionX(i) - frameData->GetRatioWidth(i));
            verticesRectangle.push_back(frameData->GetPositionY(i) - frameData->GetRatioHeight(i));

            vertexArrayRectangle = new VertexArray();
            vertexBufferRectangle = new VertexBuffer(&verticesRectangle[0],
                (unsigned int)verticesRectangle.size() * sizeof(float));
            vertexArrayRectangle->AddBuffer(*vertexBufferRectangle, layoutRectangle);            
            shaderGLRectangle->Bind();            
            shaderGLRectangle->SetUniform4f("u_Color", frameData->GetColourR(i),
                frameData->GetColourG(i), frameData->GetColourB(i), 1.0f);            
            renderer.DrawRectangle(*vertexArrayRectangle, *indexBufferRectangle, *shaderGLRectangle);
            vertexArrayRectangle->Unbind();
            vertexBufferRectangle->Unbind();
            shaderGLRectangle->Unbind();
            vertexArrayRectangle->Delete();
            vertexBufferRectangle->Delete();

        }
    }    
   
}
