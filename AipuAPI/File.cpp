#include "File.h"

File::File()
{
}

File::~File()
{
}

void File::SetNameDirectory(string name) {
	nameDirectory = name;
}

void File::SetNameFile(string name) {
	nameFile = name;
}

string File::GetNameFile() {
	return nameFile;
}
string File::GetNameLog() {
	return nameLog;
}

void File::WriteFile(string content) {
	CreateDirectory();
	string path = nameDirectory + "/" + nameFile;
	ofstream file_obj;
	file_obj.open(path, ios::app);
	file_obj.write(content.c_str(), sizeof(char)*content.size());
	file_obj.close();
	
}
void File::WriteFile(string path, string content) {
	ofstream file_obj;
	file_obj.open(path, ios::app);
	file_obj.write(content.c_str(), sizeof(char) * content.size());
	file_obj.close();
}

bool File::DeleteFile() {
	string path = nameDirectory + "/" + nameFile;
	if (remove(path.c_str()) != 0) {
		return false;
	}
	else {
		return true;
	}

}

bool File::DeleteFile(string filePath) {

	if (remove(filePath.c_str()) != 0) {
		return false;
	}
	else {
		return true;
	}

}

void File::CreateDirectory() {
	int result = _mkdir((char *)&nameDirectory);
}

string File::GetNameDirectory() {
	return nameDirectory;
}

string File::ReadFileText() {
	string result = "";
	string path = nameDirectory + "/" + nameFile;
	ifstream streamFile;
	streamFile.open(path);
	if (streamFile.good()) {
		stringstream streamContent;
		streamContent << streamFile.rdbuf();
		result = streamContent.str();
		streamFile.close();
	}
	return result;

}

string File::ReadFileText(string path) {
	string result = "";
	ifstream streamFile;
	streamFile.open(path);
	if (streamFile.good()) {
		stringstream streamContent;
		streamContent << streamFile.rdbuf();
		result = streamContent.str();
		streamFile.close();
	}
	return result;
}