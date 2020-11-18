#ifndef File_h
#define File_h

#include <iostream>
#include <fstream> 
#include <direct.h>
#include <sstream> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#define TEMP_FILE "temp.txt"
#define MAX_SIZE_IMAGE 1280

using namespace std;

class File
{
public:
	File();
	~File();
	void SetNameDirectory(string name);
	void SetNameFile(string name);
	void WriteFile(string content);
	void WriteFile(string path, string content);
	bool DeleteFile();
	bool DeleteFile(string filePath);
	void CreateDirectory();
	string GetNameDirectory();
	string GetNameFile();
	string GetNameLog();
	string ReadFileText();
	string ReadFileText(string path);
	bool GetImageSizeEx(const char* fn, int* x, int* y);
	vector<uchar> ResizeImage(const char* fileName, int width, int height);
private:
	string nameDirectory = "";
	string nameFile = "";
	string nameLog = "logChannel_";
};


#endif // !File_h

