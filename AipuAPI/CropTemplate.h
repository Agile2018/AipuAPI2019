#ifndef CropTemplate_h
#define CropTemplate_h

#include <iostream>
#include <vector>

using namespace std;

class CropTemplate
{
public:
	CropTemplate();
	~CropTemplate();
	int GetCropWidth() {
		return cropWidth;
	}

	int GetCropHeight() {
		return cropHeight;
	}

	void SetCropWidth(int size) {
		cropWidth = size;
	}

	void SetCropHeight(int size) {
		cropHeight = size;
	}

	void SetCropImageData(vector<unsigned char> dataCrop) {
		copy(dataCrop.begin(), dataCrop.end(), back_inserter(cropImageData));

	}

	vector<unsigned char> GetCropImageData() {
		return cropImageData;
	}

private:
	int cropWidth = -1, cropHeight = -1;	
	vector<unsigned char> cropImageData;
};


#endif // !CropTemplate_h

