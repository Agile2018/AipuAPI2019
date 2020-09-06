#ifndef Molded_h
#define Molded_h

#include <iostream>
#include <vector>

using namespace std;

class Molded
{
public:
	Molded();
	~Molded();
	void SetMoldImage(char* mold) {
		moldImage = mold;
		templateImage.assign(mold, mold + moldSize);
	}

	void SetCropImageData(unsigned char* moldCrop) {
		cropImageData.assign(moldCrop, moldCrop + cropLength);
	}

	vector<unsigned char> GetCropImageData() {
		return cropImageData;
	}

	vector<char> GetTemplateImage() {
		return templateImage;
	}

	void SetMoldSize(int size) {
		moldSize = size;
	}

	void SetMoldCropWidth(int size) {
		cropWidth = size;
	}

	void SetMoldCropHeight(int size) {
		cropHeight = size;
	}

	void SetMoldCropLength(int size) {
		cropLength = size;
	}

	char* GetMoldImage() {
		return moldImage;
	}

	int GetMoldSize() {
		return moldSize;
	}

	int GetMoldCropWidth() {
		return cropWidth;
	}

	int GetMoldCropHeight() {
		return cropHeight;
	}

	int GetMoldCropLength() {
		return cropLength;
	}

	void SetPathImage(string path) {
		pathImage = path;
	}

	string GetPathImage() {
		return pathImage;
	}

	int GetIdClient() {
		return idClient;
	}

	void SetIdClient(int id) {
		idClient = id;
	}

	void SetWhatDoing(int value) {
		whatDoing = value;
	}

	int GetWhatDoing() {
		return whatDoing;
	}

	void SetLocation(int value) {
		location = value;
	}

	int GetLocation() {
		return location;
	}

private:
	char* moldImage;
	vector<char> templateImage;
	vector<unsigned char> cropImageData;
	int moldSize, cropWidth, cropHeight, cropLength;
	string pathImage;
	int idClient;
	int whatDoing = 0; 
	// 0 estoy solo puedo registrarme o hacerme buscar
	//1 - Estoy con varios para registrarnos si o si.
	//2 - Estoy con varios y sólo se registraran los más lindos.
	int location = 0;
};


#endif // !Molded_h

