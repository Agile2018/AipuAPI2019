#ifndef FRAMEVIEW_H
#define FRAMEVIEW_H

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <time.h>
//#include <SOIL2.h>
#include <vector>
#include "FrameData.h"

#define NUM_FRAMES 4

class FrameView
{
public:
	FrameView();
	~FrameView();
	void RunFour(int argc, char** argv);
	void RunOne(int argc, char** argv);
	void RunTwo(int argc, char** argv);
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

	void SetImageData(int indexFrame, unsigned char* data, int size);
	void SetSides(int indexFrame, int value);
	void SetShowFrame(int indexFrame, bool value);
	

private:
	std::string nameWindow = "Recognition Videos";
	
};


#endif // !FRAMEVIEW_H
