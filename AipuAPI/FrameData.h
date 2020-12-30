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
	float GetPositionX(int index);
	float GetPositionY(int index);
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
	void TransformCoordinates();	
	void ClearImageData();

	inline void SetColourLabelR(float value) { colourLabelR = value; }
	inline void SetColourLabelG(float value) { colourLabelG = value; }
	inline void SetColourLabelB(float value) { colourLabelB = value; }

	inline float GetColourLabelR() { return colourLabelR; }
	inline float GetColourLabelG() { return colourLabelG; }
	inline float GetColourLabelB() { return colourLabelB; }	

private:
	int widthImage = 400;
	int heightImage = 300;
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
	float colourLabelR = 0.5f, colourLabelG = 0.8f, colourLabelB = 0.2f;
	std::vector<unsigned char* > flowImage;	
	
};


#endif // !FRAMEDATA_H

