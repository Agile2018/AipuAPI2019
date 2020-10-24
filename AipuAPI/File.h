#ifndef File_h
#define File_h

#include <iostream>
#include <fstream> 
#include <direct.h>
#include <sstream> 

#define TEMP_FILE "temp.txt"

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
private:
	string nameDirectory = "";
	string nameFile = "";
	string nameLog = "logChannel_";
};


#endif // !File_h

