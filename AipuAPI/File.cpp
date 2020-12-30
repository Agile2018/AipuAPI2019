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

bool File::GetImageSizeEx(const char* fn, int* x, int* y)
{
	FILE* f = fopen(fn, "rb");

	if (f == 0) return false;

	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (len < 24) {
		fclose(f);
		return false;
	}

	// Strategy:
	// reading GIF dimensions requires the first 10 bytes of the file
	// reading PNG dimensions requires the first 24 bytes of the file
	// reading JPEG dimensions requires scanning through jpeg chunks
	// In all formats, the file is at least 24 bytes big, so we'll read that always
	unsigned char buf[24]; fread(buf, 1, 24, f);


	// For JPEGs, we need to read the first 12 bytes of each chunk.
	// We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0 && buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F')
	{
		long pos = 2;
		while (buf[2] == 0xFF)
		{
			if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2 || buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA || buf[3] == 0xCB)
				break;

			pos += 2 + (buf[4] << 8) + buf[5];
			if (pos + 12 > len) break;
			fseek(f, pos, SEEK_SET);
			fread(buf + 2, 1, 12, f);
		}
	}


	fclose(f);

	// JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF)
	{
		*y = (buf[7] << 8) + buf[8];
		*x = (buf[9] << 8) + buf[10];

		return true;
	}

	// GIF: first three bytes say "GIF", next three give version number. Then dimensions
	if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
	{
		*x = buf[6] + (buf[7] << 8);
		*y = buf[8] + (buf[9] << 8);
		return true;
	}

	// PNG: the first frame is by definition an IHDR frame, which gives dimensions
	if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A && buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R')
	{
		*x = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
		*y = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);

		return true;
	}

	return false;
}

vector<uchar> File::ResizeImage(const char* fileName, int width, int height) {
	vector<uchar> bufferImage;
	int params[3] = { 0 };
	params[0] = cv::IMWRITE_JPEG_QUALITY;
	params[1] = 100;

	float relationSize = (float)width / (float)height;
	int newWidth = MAX_SIZE_IMAGE;
	int newHeight = (int)((float)newWidth / relationSize);
	cv::Mat img = cv::imread(fileName);
	cv::Mat newImg;
	cv::resize(img, newImg, cv::Size(newWidth, newHeight));	

	//cv::imwrite("Logs/some.jpg", newImg);

	bool code = cv::imencode(".jpg", newImg, bufferImage, std::vector<int>(params, params + 2));

	return bufferImage;
}

std::string File::BuildHeadLog() {

	return "Date, Channel, Channel Type (1=IP 2=File 3=Device), Source, Source Quantity, Image Size, Face Detection Confidence Detected, Face Detection Confidence Threshold, Source Quantity above Face Detection Confidence Threshold, Template Quality, Template Quality Threshold, Source Quantity over Template Quality Threshold, Function (Enroll Video Single UserID - Enroll Video Separate UserIDs - Enroll File Single UserID - Enroll File Separate UserIDs), Deduplicate Enrollment (1=yes 0=no), Result of Deduplication/Entry Search (UserID - Score), Deduplication Score_Threshold, Concatenate Enrollment Templates (0=No 1=yes), Concatenation Mode (0=auto - 1=forced), Enroll Concatenation Minimum Score Threshold, Enroll Concatenation Maximum Score Threshold, Identification Score Threshold, Verification Score Theshold, Match scores, Number of Templates Concatenated, Result\n";
}

bool File::FileLogExists() {
	string path = nameDirectory + "/" + nameFile;
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}