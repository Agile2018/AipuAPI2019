#include "FrameView.h"
//#include <mutex>

//int widthImage = 800;
//int heightImage = 400;
//int elapse = 100;
GLuint idTexture1 = 0, idTexture2 = 0, idTexture3 = 0, idTexture4 = 0;
GLushort pattern = 0x5555;
GLint factorStipple = 10;
GLfloat lineWidth = 3.0f;
GLfloat colourRfo = 0.0, colourGfo = 0.0, colourBfo = 0.0;
GLfloat colourRft = 0.0, colourGft = 0.0, colourBft = 0.0;
GLfloat colourRfth = 0.0, colourGfth = 0.0, colourBfth = 0.0;
GLfloat colourRff = 0.0, colourGff = 0.0, colourBff = 0.0;
std::string lblOne = "Video 1";
std::string lblTwo = "Video 2";
std::string lblThree = "Video 3";
std::string lblFour = "Video 4";
FrameData* frameOne = new FrameData();
FrameData* frameTwo = new FrameData();
FrameData* frameThree = new FrameData();
FrameData* frameFour = new FrameData();

//FrameData* frames = new FrameData[NUM_FRAMES];
//FrameData frames[NUM_FRAMES];
//float colourR[NUM_RECTANGLES] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
//float colourG[NUM_RECTANGLES] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
//float colourB[NUM_RECTANGLES] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
//float ratioWidth[NUM_RECTANGLES] = {};
//float ratioHeight[NUM_RECTANGLES] = {};
//float transforX[NUM_RECTANGLES] = {};
//float transforY[NUM_RECTANGLES] = {};
//float positionX[NUM_RECTANGLES] = {};
//float positionY[NUM_RECTANGLES] = {};
//float halfScreenWidth = 0.0f;
//float halfScreenHeight = 0.0f;
//std::vector<unsigned char* > flowImage;
//std::mutex mtx;
int handleWindow, windowOne, windowTwo, windowThree, windowFour;
//bool showFrame = true;
//int sides = 32;

FrameView::FrameView()
{
    
}

FrameView::~FrameView()
{
    delete frameOne;
    delete frameTwo;
    delete frameThree;
    delete frameFour;
}

void FrameView::SetWidthImage(int index, int value) {
    switch (index)
    {
    case 1:
        frameOne->SetWidthImage(value);
        break;
    case 2:
        frameTwo->SetWidthImage(value);
        break;
    case 3:
        frameThree->SetWidthImage(value);
        break;
    case 4:
        frameFour->SetWidthImage(value);
        break;
    default:
        break;
    }
        
}

void FrameView::SetHeightImage(int index, int value) {
    switch (index)
    {
    case 1:
        frameOne->SetHeightImage(value);
        break;
    case 2:
        frameTwo->SetHeightImage(value);
        break;
    case 3:
        frameThree->SetHeightImage(value);
        break;
    case 4:
        frameFour->SetHeightImage(value);
        break;
    default:
        break;
    }
        
}

void FrameView::SetElapse(int index, int value) {
    switch (index)
    {
    case 1:
        std::cout << "ELAPSE ONE: " << value << std::endl;
        frameOne->SetElapse(value);
        break;
    case 2:
        std::cout << "ELAPSE TWO: " << value << std::endl;
        frameTwo->SetElapse(value);
        break;
    case 3:
        std::cout << "ELAPSE THREE: " << value << std::endl;
        frameThree->SetElapse(value);
        break;
    case 4:
        std::cout << "ELAPSE FOUR: " << value << std::endl;
        frameFour->SetElapse(value);
        break;
    default:
        break;
    }
       
}

void FrameView::SetFactorStipple(GLint value) {
    factorStipple = value;
}

void FrameView::SetLineWidth(GLfloat value) {
    lineWidth = value;
}

void FrameView::SetColourR(int indexFrame, int indexFigure, float value) {
    switch (indexFrame)
    {
    case 1:
        frameOne->SetColourR(indexFigure, value);
        break;
    case 2:
        frameTwo->SetColourR(indexFigure, value);
        break;
    case 3:
        frameThree->SetColourR(indexFigure, value);
        break;
    case 4:
        frameFour->SetColourR(indexFigure, value);
        break;
    default:
        break;
    }
    
    
}

void FrameView::SetColourG(int indexFrame, int indexFigure, float value) {
    switch (indexFrame)
    {
    case 1:
        frameOne->SetColourG(indexFigure, value);
        break;
    case 2:
        frameTwo->SetColourG(indexFigure, value);
        break;
    case 3:
        frameThree->SetColourG(indexFigure, value);
        break;
    case 4:
        frameFour->SetColourG(indexFigure, value);
        break;
    default:
        break;
    }
    
}

void FrameView::SetColourB(int indexFrame, int indexFigure, float value) {
    switch (indexFrame)
    {
    case 1:
        frameOne->SetColourB(indexFigure, value);
        break;
    case 2:
        frameTwo->SetColourB(indexFigure, value);
        break;
    case 3:
        frameThree->SetColourB(indexFigure, value);
        break;
    case 4:
        frameFour->SetColourB(indexFigure, value);
        break;
    default:
        break;
    }
    
}

void FrameView::SetRatioWidth(int indexFrame, int indexFigure, float value) {
    switch (indexFrame)
    {
    case 1:
        frameOne->SetRatioWidth(indexFigure, value);
        break;
    case 2:
        frameTwo->SetRatioWidth(indexFigure, value);
        break;
    case 3:
        frameThree->SetRatioWidth(indexFigure, value);
        break;
    case 4:
        frameFour->SetRatioWidth(indexFigure, value);
        break;
    default:
        break;
    }
    
    
}

void FrameView::SetRatioHeight(int indexFrame, int indexFigure, float value) {
    switch (indexFrame)
    {
    case 1:
        frameOne->SetRatioHeight(indexFigure, value);
        break;
    case 2:
        frameTwo->SetRatioHeight(indexFigure, value);
        break;
    case 3:
        frameThree->SetRatioHeight(indexFigure, value);
        break;
    case 4:
        frameFour->SetRatioHeight(indexFigure, value);
        break;
    default:
        break;
    }
    
}

void FrameView::SetPositionX(int indexFrame, int indexFigure, float value) {
    switch (indexFrame)
    {
    case 1:
        frameOne->SetPositionX(indexFigure, value);
        break;
    case 2:
        frameTwo->SetPositionX(indexFigure, value);
        break;
    case 3:
        frameThree->SetPositionX(indexFigure, value);
        break;
    case 4:
        frameFour->SetPositionX(indexFigure, value);
        break;
    default:
        break;
    }
        
}
void FrameView::SetPositionY(int indexFrame, int indexFigure, float value) {
    switch (indexFrame)
    {
    case 1:
        frameOne->SetPositionY(indexFigure, value);
        break;
    case 2:
        frameTwo->SetPositionY(indexFigure, value);
        break;
    case 3:
        frameThree->SetPositionY(indexFigure, value);
        break;
    case 4:
        frameFour->SetPositionY(indexFigure, value);
        break;
    default:
        break;
    }
    
}

int FrameView::GetWidthImage(int index) {
    switch (index)
    {
    case 1:
        return frameOne->GetWidthImage();
        break;
    case 2:
        return frameTwo->GetWidthImage();
        break;
    case 3:
        return frameThree->GetWidthImage();
        break;
    case 4:
        return frameFour->GetWidthImage();
        break;
    default:
        return 0;
        break;
    }
    
}

int FrameView::GetHeightImage(int index) {
    switch (index)
    {
    case 1:
        return frameOne->GetHeightImage();
        break;
    case 2:
        return frameTwo->GetHeightImage();
        break;
    case 3:
        return frameThree->GetHeightImage();
        break;
    case 4:
        return frameFour->GetHeightImage();
        break;
    default:
        return 0;
        break;
    }
    
}

void FrameView::SetImageData(int indexFrame, unsigned char* data, int size) {
    switch (indexFrame)
    {
    case 1:
        frameOne->SetImageData(data, size);
        break;
    case 2:
        frameTwo->SetImageData(data, size);
        break;
    case 3:
        frameThree->SetImageData(data, size);
        break;
    case 4:
        frameFour->SetImageData(data, size);
        break;
    default:
        break;
    }
    
}

void FrameView::SetSides(int indexFrame, int value) {
    switch (indexFrame)
    {
    case 1:
        frameOne->SetSides(value);
        break;
    case 2:
        frameTwo->SetSides(value);
        break;
    case 3:
        frameThree->SetSides(value);
        break;
    case 4:
        frameFour->SetSides(value);
        break;
    default:
        break;
    }
    
}

void FrameView::SetShowFrame(int indexFrame, bool value) {
    switch (indexFrame)
    {
    case 1:
        frameOne->SetShowFrame(value);
        
        break;
    case 2:
        frameTwo->SetShowFrame(value);
        
        break;
    case 3:
        frameThree->SetShowFrame(value);
        
        break;
    case 4:
        frameFour->SetShowFrame(value);
        
        break;
    default:
        break;
    }
    
}

void InitTextureOne() {
    glGenTextures(1, &idTexture1);
    glBindTexture(GL_TEXTURE_2D, idTexture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameOne->GetWidthImage(), 
        frameOne->GetHeightImage(), 0,
        GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

void InitTextureTwo() {
    glGenTextures(1, &idTexture2);
    glBindTexture(GL_TEXTURE_2D, idTexture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameTwo->GetWidthImage(),
        frameTwo->GetHeightImage(), 0,
        GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

void InitTextureThree() {
    glGenTextures(1, &idTexture3);
    glBindTexture(GL_TEXTURE_2D, idTexture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameThree->GetWidthImage(),
        frameThree->GetHeightImage(), 0,
        GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

void InitTextureFour() {
    glGenTextures(1, &idTexture4);
    glBindTexture(GL_TEXTURE_2D, idTexture4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameFour->GetWidthImage(),
        frameFour->GetHeightImage(), 0,
        GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

void LoadImageOnTextureOne() {
    if (frameOne->mtx.try_lock())
    {
        unsigned char* image = frameOne->LoadImageForTexture();
        if (image != nullptr)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameOne->GetWidthImage(),
                frameOne->GetHeightImage(), 0,
                GL_RGB, GL_UNSIGNED_BYTE, image);
            
        }
        image = nullptr;
        free(image);
        frameOne->mtx.unlock();
    }   

}

void LoadImageOnTextureTwo() {
    if (frameTwo->mtx.try_lock())
    {
        unsigned char* image = frameTwo->LoadImageForTexture();
        if (image != nullptr)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameTwo->GetWidthImage(),
                frameTwo->GetHeightImage(), 0,
                GL_RGB, GL_UNSIGNED_BYTE, image);
            
        }
        image = nullptr;
        free(image);
        frameTwo->mtx.unlock();
    }

}

void LoadImageOnTextureThree() {
    if (frameThree->mtx.try_lock())
    {
        unsigned char* image = frameThree->LoadImageForTexture();
        if (image != nullptr)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameThree->GetWidthImage(),
                frameThree->GetHeightImage(), 0,
                GL_RGB, GL_UNSIGNED_BYTE, image);
            
        }
        image = nullptr;
        free(image);
        frameThree->mtx.unlock();
    }

}

void LoadImageOnTextureFour() {
    if (frameFour->mtx.try_lock())
    {
        unsigned char* image = frameFour->LoadImageForTexture();
        if (image != nullptr)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameFour->GetWidthImage(),
                frameFour->GetHeightImage(), 0,
                GL_RGB, GL_UNSIGNED_BYTE, image);
            
        }
        image = nullptr;
        free(image);
        frameFour->mtx.unlock();
    }

}

void DrawRectangleOne() {

    for (int i = 0; i < NUM_RECTANGLES; i++)
    {
        if (frameOne->GetRatioWidth(i) != 0.0f)
        {
            glLineStipple(factorStipple, pattern);
            glLineWidth(lineWidth);
            glBegin(GL_LINE_STRIP); //GL_LINE_STRIP GL_LINES GL_LINE_LOOP
            glColor3f(frameOne->GetColourR(i), frameOne->GetColourG(i),
                frameOne->GetColourB(i));
            glVertex2f(frameOne->GetTransforX(i) - frameOne->GetRatioWidth(i),
                frameOne->GetTransforY(i) + frameOne->GetRatioHeight(i));
            glVertex2f(frameOne->GetTransforX(i) + frameOne->GetRatioWidth(i),
                frameOne->GetTransforY(i) + frameOne->GetRatioHeight(i));
            glVertex2f(frameOne->GetTransforX(i) + frameOne->GetRatioWidth(i),
                frameOne->GetTransforY(i) - frameOne->GetRatioHeight(i));
            glVertex2f(frameOne->GetTransforX(i) - frameOne->GetRatioWidth(i),
                frameOne->GetTransforY(i) - frameOne->GetRatioHeight(i));
            glVertex2f(frameOne->GetTransforX(i) - frameOne->GetRatioWidth(i),
                frameOne->GetTransforY(i) + frameOne->GetRatioHeight(i));
            glEnd();
        }
    }

}

void DrawRectangleTwo() {

    for (int i = 0; i < NUM_RECTANGLES; i++)
    {
        if (frameTwo->GetRatioWidth(i) != 0.0f)
        {
            glLineStipple(factorStipple, pattern);
            glLineWidth(lineWidth);
            glBegin(GL_LINE_STRIP); //GL_LINE_STRIP GL_LINES GL_LINE_LOOP
            glColor3f(frameTwo->GetColourR(i), frameTwo->GetColourG(i),
                frameTwo->GetColourB(i));
            glVertex2f(frameTwo->GetTransforX(i) - frameTwo->GetRatioWidth(i),
                frameTwo->GetTransforY(i) + frameTwo->GetRatioHeight(i));
            glVertex2f(frameTwo->GetTransforX(i) + frameTwo->GetRatioWidth(i),
                frameTwo->GetTransforY(i) + frameTwo->GetRatioHeight(i));
            glVertex2f(frameTwo->GetTransforX(i) + frameTwo->GetRatioWidth(i),
                frameTwo->GetTransforY(i) - frameTwo->GetRatioHeight(i));
            glVertex2f(frameTwo->GetTransforX(i) - frameTwo->GetRatioWidth(i),
                frameTwo->GetTransforY(i) - frameTwo->GetRatioHeight(i));
            glVertex2f(frameTwo->GetTransforX(i) - frameTwo->GetRatioWidth(i),
                frameTwo->GetTransforY(i) + frameTwo->GetRatioHeight(i));
            glEnd();
        }
    }

}

void DrawRectangleThree() {

    for (int i = 0; i < NUM_RECTANGLES; i++)
    {
        if (frameThree->GetRatioWidth(i) != 0.0f)
        {
            glLineStipple(factorStipple, pattern);
            glLineWidth(lineWidth);
            glBegin(GL_LINE_STRIP); //GL_LINE_STRIP GL_LINES GL_LINE_LOOP
            glColor3f(frameThree->GetColourR(i), frameThree->GetColourG(i),
                frameThree->GetColourB(i));
            glVertex2f(frameThree->GetTransforX(i) - frameThree->GetRatioWidth(i),
                frameThree->GetTransforY(i) + frameThree->GetRatioHeight(i));
            glVertex2f(frameThree->GetTransforX(i) + frameThree->GetRatioWidth(i),
                frameThree->GetTransforY(i) + frameThree->GetRatioHeight(i));
            glVertex2f(frameThree->GetTransforX(i) + frameThree->GetRatioWidth(i),
                frameThree->GetTransforY(i) - frameThree->GetRatioHeight(i));
            glVertex2f(frameThree->GetTransforX(i) - frameThree->GetRatioWidth(i),
                frameThree->GetTransforY(i) - frameThree->GetRatioHeight(i));
            glVertex2f(frameThree->GetTransforX(i) - frameThree->GetRatioWidth(i),
                frameThree->GetTransforY(i) + frameThree->GetRatioHeight(i));
            glEnd();
        }
    }

}

void DrawRectangleFour() {

    for (int i = 0; i < NUM_RECTANGLES; i++)
    {
        if (frameFour->GetRatioWidth(i) != 0.0f)
        {
            glLineStipple(factorStipple, pattern);
            glLineWidth(lineWidth);
            glBegin(GL_LINE_STRIP); //GL_LINE_STRIP GL_LINES GL_LINE_LOOP
            glColor3f(frameFour->GetColourR(i), frameFour->GetColourG(i),
                frameFour->GetColourB(i));
            glVertex2f(frameFour->GetTransforX(i) - frameFour->GetRatioWidth(i),
                frameFour->GetTransforY(i) + frameFour->GetRatioHeight(i));
            glVertex2f(frameFour->GetTransforX(i) + frameFour->GetRatioWidth(i),
                frameFour->GetTransforY(i) + frameFour->GetRatioHeight(i));
            glVertex2f(frameFour->GetTransforX(i) + frameFour->GetRatioWidth(i),
                frameFour->GetTransforY(i) - frameFour->GetRatioHeight(i));
            glVertex2f(frameFour->GetTransforX(i) - frameFour->GetRatioWidth(i),
                frameFour->GetTransforY(i) - frameFour->GetRatioHeight(i));
            glVertex2f(frameFour->GetTransforX(i) - frameFour->GetRatioWidth(i),
                frameFour->GetTransforY(i) + frameFour->GetRatioHeight(i));
            glEnd();
        }
    }

}

//void DrawRegularPolygon(int index) {
//    for (int i = 0; i < NUM_RECTANGLES; i++) {
//        if (frames[index].GetRatioWidth(i) != 0.0f) {
//            float xi = 0.0f, yi = 0.0f, xf = 0.0f, yf = 0.0f;
//
//            glLineStipple(factorStipple, pattern);
//            glLineWidth(lineWidth);
//            glBegin(GL_LINE_STRIP);
//            glColor3f(frames[index].GetColourR(i), frames[index].GetColourG(i),
//                frames[index].GetColourB(i));
//            for (int j = 0; j < frames[index].GetSides(); j++)
//            {
//                float ang = 6.2832f * j / (float)frames[index].GetSides(); // PI * 2.0
//                xf = frames[index].GetTransforX(i) + (cos(ang) * frames[index].GetRatioWidth(i));
//                yf = frames[index].GetTransforY(i) + (-sin(ang) * frames[index].GetRatioHeight(i));
//                if (j != 0)
//                {
//
//                    glVertex2f(xi, yi);
//                    glVertex2f(xf, yf);
//                }
//                xi = xf;
//                yi = yf;
//            }
//            glEnd();
//        }
//    }
//}



void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();
}


void DisplayFrameOne() {

    glClear(GL_COLOR_BUFFER_BIT);

    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexture1);

    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);  // Bottom Right Of The Texture and Quad    
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);  // top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);  // top Left Of The Texture and Quad

    glEnd();
    glDisable(GL_TEXTURE_2D);

    DrawRectangleOne();
   
    glEnable(GL_BLEND);
    glColor3f(colourRfo, colourGfo, colourBfo);
    glRasterPos2f(-0.9f, 0.8f);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,
        (unsigned char*)lblOne.c_str());
    glDisable(GL_BLEND);
    
    //DrawRegularPolygon();
    glutSwapBuffers();

}

void DisplayFrameTwo() {

    glClear(GL_COLOR_BUFFER_BIT);

    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexture2);

    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);  // Bottom Right Of The Texture and Quad    
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);  // top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);  // top Left Of The Texture and Quad

    glEnd();
    glDisable(GL_TEXTURE_2D);

    DrawRectangleTwo();

    glEnable(GL_BLEND);
    glColor3f(colourRft, colourGft, colourBft);
    glRasterPos2f(-0.9f, 0.8f);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,
        (unsigned char*)lblTwo.c_str());
    glDisable(GL_BLEND);

    //DrawRegularPolygon();
    glutSwapBuffers();

}

void DisplayFrameThree() {

    glClear(GL_COLOR_BUFFER_BIT);

    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexture3);

    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);  // Bottom Right Of The Texture and Quad    
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);  // top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);  // top Left Of The Texture and Quad

    glEnd();
    glDisable(GL_TEXTURE_2D);

    DrawRectangleThree();

    glEnable(GL_BLEND);
    glColor3f(colourRfth, colourGfth, colourBfth);
    glRasterPos2f(-0.9f, 0.8f);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,
        (unsigned char*)lblThree.c_str());
    glDisable(GL_BLEND);

    //DrawRegularPolygon();
    glutSwapBuffers();

}

void DisplayFrameFour() {

    glClear(GL_COLOR_BUFFER_BIT);

    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexture4);

    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);  // Bottom Right Of The Texture and Quad    
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);  // top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);  // top Left Of The Texture and Quad

    glEnd();
    glDisable(GL_TEXTURE_2D);

    DrawRectangleFour();

    glEnable(GL_BLEND);
    glColor3f(colourRff, colourGff, colourBff);
    glRasterPos2f(-0.9f, 0.8f);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,
        (unsigned char*)lblFour.c_str());
    glDisable(GL_BLEND);

    //DrawRegularPolygon();
    glutSwapBuffers();

}

void ToggleGlutWindowCloseBox()
{    
    HWND hwndGlut;
    hwndGlut = GetForegroundWindow();   
    SetClassLongPtr(hwndGlut, GCL_STYLE,
        GetClassLongPtr(hwndGlut, GCL_STYLE) | CS_NOCLOSE);
}

void RefreshFrameOne(int value) {
    static int disabledClose = 0;
    glutSetWindow(windowOne);
    if (frameOne->GetShowFrame())
    {
        
        frameOne->TransformCoordinates();
        LoadImageOnTextureOne();
        if (disabledClose == 0)
        {
            disabledClose = 1;
            ToggleGlutWindowCloseBox();
        }
        
    }
    glutTimerFunc(frameOne->GetElapse(), RefreshFrameOne, 1);
    glutPostRedisplay();
}

void RefreshFrameTwo(int value) {
    glutSetWindow(windowTwo);
    if (frameTwo->GetShowFrame())
    {
        
        frameTwo->TransformCoordinates();
        LoadImageOnTextureTwo();
        
    }
    glutTimerFunc(frameTwo->GetElapse(), RefreshFrameTwo, 1);
    glutPostRedisplay();
}

void RefreshFrameThree(int value) {
    glutSetWindow(windowThree);
    if (frameThree->GetShowFrame())
    {        
        frameThree->TransformCoordinates();
        LoadImageOnTextureThree();
        
    }
    glutTimerFunc(frameThree->GetElapse(), RefreshFrameThree, 1);
    glutPostRedisplay();
}

void RefreshFrameFour(int value) {
    glutSetWindow(windowFour);
    if (frameFour->GetShowFrame())
    {
        
        frameFour->TransformCoordinates();
        LoadImageOnTextureFour();
        
    }
    glutTimerFunc(frameFour->GetElapse(), RefreshFrameFour, 1);
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    int width = 50;
    int height = 50;

    glViewport(0, 0, w, h);
    if (w > 50) {
        width = (w - 2 * GAP);
    }
    else {
        width = 10;
    }
    if (h > 50) {
        height = (h - 2 * GAP); // (h - 3 * GAP) / 2;
    }
    else {
        height = 10;
    }
    glutSetWindow(windowOne);
    glutPositionWindow(GAP, GAP);
    glutReshapeWindow(width, height);    

}

void reshapeTwo(int w, int h)
{
    int width = 50;
    int height = 50;

    glViewport(0, 0, w, h);
    if (w > 50) {
        width = (w - 3 * GAP) / 2;
    }
    else {
        width = 10;
    }
    if (h > 50) {
        height = (h - 2 * GAP); // (h - 3 * GAP) / 2;
    }
    else {
        height = 10;
    }
    glutSetWindow(windowOne);
    glutPositionWindow(GAP, GAP);
    glutReshapeWindow(width, height);
    glutSetWindow(windowTwo);
    glutPositionWindow(GAP + width + GAP, GAP);
    glutReshapeWindow(width, height);
    
}

void reshapeFour(int w, int h)
{
    int width = 50;
    int height = 50;

    glViewport(0, 0, w, h);
    if (w > 50) {
        width = (w - 3 * GAP) / 2;
    }
    else {
        width = 10;
    }
    if (h > 50) {
        height = (h - 3 * GAP) / 2;
    }
    else {
        height = 10;
    }
    glutSetWindow(windowOne);
    glutPositionWindow(GAP, GAP);
    glutReshapeWindow(width, height);
    glutSetWindow(windowTwo);
    glutPositionWindow(GAP + width + GAP, GAP);
    glutReshapeWindow(width, height);
    glutSetWindow(windowThree);
    glutPositionWindow(GAP, GAP + height + GAP);
    glutReshapeWindow(width, height);
    glutSetWindow(windowFour);
    glutPositionWindow(GAP + width + GAP, GAP + height + GAP);
    glutReshapeWindow(width, height);
}

void FrameView::SetColourTextFrameOne(float red, float green, float blue) {
    colourRfo = red;
    colourGfo = green;
    colourBfo = blue;
}

void FrameView::SetColourTextFrameTwo(float red, float green, float blue) {
    colourRft = red;
    colourGft = green;
    colourBft = blue;
}

void FrameView::SetColourTextFrameThree(float red, float green, float blue) {
    colourRfth = red;
    colourGfth = green;
    colourBfth = blue;
}

void FrameView::SetColourTextFrameFour(float red, float green, float blue) {
    colourRff = red;
    colourGff = green;
    colourBff = blue;
}

void FrameView::RunFour(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB); // GLUT_DOUBLE | GLUT_RGB   GLUT_SINGLE   GLUT_RGBA | GLUT_DOUBLE
    glutInitWindowSize(400, 400);                    // window size    
    handleWindow = glutCreateWindow(nameWindow.c_str());    // message displayed on top bar window    
    glutSetWindow(handleWindow);         
    glutDisplayFunc(Display);
    glutReshapeFunc(reshapeFour);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    windowOne = glutCreateSubWindow(handleWindow, 10, 10, 90, 90);
    glEnable(GL_LINE_STIPPLE);
    glutDisplayFunc(DisplayFrameOne);    
    glutTimerFunc(frameOne->GetElapse(), RefreshFrameOne, 1);
    InitTextureOne();

    windowTwo = glutCreateSubWindow(handleWindow, 110, 10, 90, 90);
    glEnable(GL_LINE_STIPPLE);
    glutDisplayFunc(DisplayFrameTwo);
    glutTimerFunc(frameTwo->GetElapse(), RefreshFrameTwo, 1);
    InitTextureTwo();
    
    windowThree = glutCreateSubWindow(handleWindow, 10, 110, 90, 90);
    glEnable(GL_LINE_STIPPLE);
    glutDisplayFunc(DisplayFrameThree);
    glutTimerFunc(frameThree->GetElapse(), RefreshFrameThree, 1);
    InitTextureThree();

    windowFour = glutCreateSubWindow(handleWindow, 110, 110, 90, 90);
    glEnable(GL_LINE_STIPPLE);
    glutDisplayFunc(DisplayFrameFour);
    glutTimerFunc(frameFour->GetElapse(), RefreshFrameFour, 1);
    InitTextureFour();

    glutMainLoop();
}

void FrameView::RunOne(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB); // GLUT_DOUBLE | GLUT_RGB   GLUT_SINGLE   GLUT_RGBA | GLUT_DOUBLE
    glutInitWindowSize(400, 400);                    // window size        
    handleWindow = glutCreateWindow(nameWindow.c_str());    // message displayed on top bar window    
    
    glutSetWindow(handleWindow);

    glutDisplayFunc(Display);
    glutReshapeFunc(reshape);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    windowOne = glutCreateSubWindow(handleWindow, 10, 10, 90, 90);
    glEnable(GL_LINE_STIPPLE);
    glutDisplayFunc(DisplayFrameOne);
    glutTimerFunc(frameOne->GetElapse(), RefreshFrameOne, 1);
    InitTextureOne();        
    glutMainLoop();
}

void FrameView::RunTwo(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB); // GLUT_DOUBLE | GLUT_RGB   GLUT_SINGLE   GLUT_RGBA | GLUT_DOUBLE
    glutInitWindowSize(400, 200);                    // window size    
    handleWindow = glutCreateWindow(nameWindow.c_str());    // message displayed on top bar window    
    glutSetWindow(handleWindow);
    glutDisplayFunc(Display);
    glutReshapeFunc(reshapeTwo);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    windowOne = glutCreateSubWindow(handleWindow, 10, 10, 90, 90);
    glEnable(GL_LINE_STIPPLE);
    glutDisplayFunc(DisplayFrameOne);
    glutTimerFunc(frameOne->GetElapse(), RefreshFrameOne, 1);
    InitTextureOne();

    windowTwo = glutCreateSubWindow(handleWindow, 110, 10, 90, 90);
    glEnable(GL_LINE_STIPPLE);
    glutDisplayFunc(DisplayFrameTwo);
    glutTimerFunc(frameTwo->GetElapse(), RefreshFrameTwo, 1);
    InitTextureTwo();
    glutMainLoop();
}

void FrameView::DestroyWindow() {    
    //int win = glutGetWindow();
    glutDestroyWindow(handleWindow);
}

