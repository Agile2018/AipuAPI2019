#include "FrameData.h"

FrameData::FrameData()
{
}

FrameData::~FrameData()
{
    flowImage.clear();
}

void FrameData::SetWidthImage(int value) {
    widthImage = value;
    halfScreenWidth = (float)value / 2.0f;
}

void FrameData::SetHeightImage(int value) {
    heightImage = value;
    halfScreenHeight = (float)value / 2.0f;
}

void FrameData::SetElapse(int value) {
    elapse = value;
    
}

int FrameData::GetElapse() {
    return elapse;
}

void FrameData::SetColourR(int index, float value) {
    colourR[index] = value;
}
float FrameData::GetColourR(int index) {
    return colourR[index];
}

void FrameData::SetColourG(int index, float value) {
    colourG[index] = value;
}
float FrameData::GetColourG(int index) {
    return colourG[index];
}
void FrameData::SetColourB(int index, float value) {
    colourB[index] = value;
}
float FrameData::GetColourB(int index) {
    return colourB[index];
}
void FrameData::SetRatioWidth(int index, float value) {
    ratioWidth[index] = value;
}
float FrameData::GetRatioWidth(int index) {
    return ratioWidth[index];
}

void FrameData::SetRatioHeight(int index, float value) {
    ratioHeight[index] = value;
}

float FrameData::GetRatioHeight(int index) {
    return ratioHeight[index];
}

void FrameData::SetPositionX(int index, float value) {
    positionX[index] = value;
}
void FrameData::SetPositionY(int index, float value) {
    positionY[index] = value;
}

float FrameData::GetPositionX(int index)
{
    return positionX[index];
}

float FrameData::GetPositionY(int index)
{
    return positionY[index];
}

float FrameData::GetTransforX(int index) {
    return transforX[index];
}

float FrameData::GetTransforY(int index) {
    return transforY[index];
}

int FrameData::GetWidthImage() {
    return widthImage;
}

int FrameData::GetHeightImage() {
    return heightImage;
}

void FrameData::ClearImageData() {
    if (!flowImage.empty()) {
        SOIL_free_image_data(flowImage.back());
        flowImage.pop_back();
    }
}

void FrameData::SetImageData(unsigned char* data, int size) {
    int w, h, c;
    mtx.lock();

    if (!flowImage.empty()) {
        SOIL_free_image_data(flowImage.back());
        flowImage.pop_back();
    }
    flowImage.push_back(SOIL_load_image_from_memory(data,
        size, &w, &h, &c, SOIL_LOAD_RGB));
    mtx.unlock();
}

void FrameData::TransformCoordinates() {
    float rx = 100.0f / halfScreenWidth;
    float ry = 100.0f / halfScreenHeight;

    for (int i = 0; i < NUM_RECTANGLES; i++)
    {
        if (positionX[i] < halfScreenWidth && positionX[i] != 0.0f)
        {
            float px = positionX[i] * rx;
            transforX[i] = -(1.0f - (px / 100.0f));
        }
        else if (positionX[i] > halfScreenWidth)
        {
            float nxi = positionX[i] - halfScreenWidth;
            float px = nxi * rx;
            transforX[i] = (px / 100.0f);
        }
        else
        {
            transforX[i] = 0.0f;
        }

        if (positionY[i] < halfScreenHeight && positionY[i] != 0.0f)
        {
            float py = positionY[i] * ry;
            transforY[i] = (1.0f - (py / 100.0f));
        }
        else if (positionY[i] > halfScreenHeight)
        {
            float nyi = positionY[i] - halfScreenHeight;
            float py = nyi * ry;
            transforY[i] = -(py / 100.0f);
        }
        else
        {
            transforY[i] = 0.0f;
        }
    }

}

unsigned char* FrameData::LoadImageForTexture() {
    if (!flowImage.empty())
    {
        return flowImage.back();

    }
    return nullptr;
}

