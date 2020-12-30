#ifndef FRAMEVIEW_H
#define FRAMEVIEW_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLFrame.h"
#include <time.h>
#include <vector>
#include "FrameData.h"

#define NUM_FRAMES 4

class FrameView
{
public:
	FrameView();
	~FrameView();
	int RunFour();
	int RunOne();
	int RunTwo();
	int RunThree();
	void DestroyWindow();
	void SetPositionX(int indexFrame, int indexFigure, float value);
	void SetPositionY(int indexFrame, int indexFigure, float value);
	void SetWidthImage(int index, int value);

	void SetHeightImage(int index, int value);

	int GetWidthImage(int index);

	int GetHeightImage(int index);

	void SetRatioWidth(int indexFrame, int indexFigure, float value);

	void SetRatioHeight(int indexFrame, int indexFigure, float value);

	void SetLineWidth(GLfloat value);

	void SetFactorStipple(GLint value);

	void SetColourR(int indexFrame, int indexFigure, float value);

	void SetColourG(int indexFrame, int indexFigure, float value);

	void SetColourB(int indexFrame, int indexFigure, float value);

	void SetElapse(int index, int value);

	void SetNameWindow(std::string name) {
		nameWindow = name;
	}

	void SetIndexCurrent(int value);
	void SetImageData(int indexFrame, unsigned char* data, int size);
	void SetColourLabelFrame(int indexFrame, float red, float green, float blue);
	/*void SetColourTextFrameOne(float red, float green, float blue);
	void SetColourTextFrameTwo(float red, float green, float blue);
	void SetColourTextFrameThree(float red, float green, float blue);
	void SetColourTextFrameFour(float red, float green, float blue);*/
private:
	std::string nameWindow = "Recognition Videos";
	
};


#endif // !FRAMEVIEW_H
