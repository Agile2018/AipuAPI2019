#ifndef FRAMEDATA_H
#define FRAMEDATA_H

#include <iostream>
#include <SOIL2.h>
#include <vector>
#include <mutex>

#define NUM_RECTANGLES	5
#define GAP 10

class FrameData
{
public:
	FrameData();
	~FrameData();
	std::mutex mtx;
	void SetPositionX(int index, float value);
	void SetPositionY(int index, float value);
	void SetWidthImage(int value);

	void SetHeightImage(int value);

	int GetWidthImage();

	int GetHeightImage();

	void SetRatioWidth(int index, float value);
	float GetRatioWidth(int index);

	void SetRatioHeight(int index, float value);
	float GetRatioHeight(int index);

	float GetTransforX(int index);

	float GetTransforY(int index);

	void SetColourR(int index, float value);
	float GetColourR(int index);

	void SetColourG(int index, float value);
	float GetColourG(int index);

	void SetColourB(int index, float value);
	float GetColourB(int index);

	void SetElapse(int value);
	int GetElapse();
	void SetImageData(unsigned char* data, int size);
	unsigned char* LoadImageForTexture();
	void SetSides(int value);	
	int GetSides();
	void TransformCoordinates();
	void SetShowFrame(bool value);
	bool GetShowFrame();

private:
	int widthImage = 800;
	int heightImage = 450;
	int elapse = 100;
	float colourR[NUM_RECTANGLES] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	float colourG[NUM_RECTANGLES] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	float colourB[NUM_RECTANGLES] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	float ratioWidth[NUM_RECTANGLES] = {};
	float ratioHeight[NUM_RECTANGLES] = {};
	float transforX[NUM_RECTANGLES] = {};
	float transforY[NUM_RECTANGLES] = {};
	float positionX[NUM_RECTANGLES] = {};
	float positionY[NUM_RECTANGLES] = {};
	float halfScreenWidth = 0.0f;
	float halfScreenHeight = 0.0f;
	std::vector<unsigned char* > flowImage;
	int sides = 32;
	bool showFrame = false;
};


#endif // !FRAMEDATA_H

