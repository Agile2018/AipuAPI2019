#ifndef FACEIDENTIFY_H
#define FACEIDENTIFY_H

#include "ErrorIdKitLib.h"
#include "Molded.h"
#include "User.h"
//#include "Format.h"
#include "FaceIdkit.h"
#include <random>
#include "CropTemplate.h"
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include "Base64.h"

class FaceIndentify
{
public:
	FaceIndentify();
	~FaceIndentify();
	void LoadConnection();		
	void CloseConnection();	
	void ForkTask(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client);
	bool GetFlagEnroll() {
		return flagEnroll;
	}
	
	void SetStringJSON(string stringJson) {
		faceIdkit->configuration->SetStringJSON(stringJson);
	}	

	void ParseJSONToObject() {
		faceIdkit->configuration->ParseJSONToObject();
	}

	void ResetEnrollVideo(int value) {
		
		countModelSendVideo = value;
		
	}

	void AddUserEnrollVideo();

	bool GetConnectionIdentification() {
		return connectionIdentification;
	}		

	void SetIsFinishLoadFiles(bool value) {
		isFinishLoadFiles = value;
	}

	bool GetIsFinishLoadFiles() {
		return isFinishLoadFiles;
	}

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
	Rx::subject<User*> userSubject;
	Rx::observable<User*> observableUser = userSubject.get_observable();
	Rx::subject<string> cropImageSubject;
	Rx::observable<string> observableCropImage = cropImageSubject.get_observable();
private:

	ErrorIdKitLib* error = new ErrorIdKitLib();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	Rx::subscriber<User*> shootUser = userSubject.get_subscriber();
	Rx::subscriber<string> shootCropImage = cropImageSubject.get_subscriber();
	bool flagEnroll = false;
	//Format* format = new Format();
	FaceIdkit* faceIdkit = new FaceIdkit();
	File* file = new File();
	Configuration* turnIntoLogToJSON = new Configuration();
	string tracerImage;
	string tracerPrevImage = "";
	string tracerMatch = "";	
	string collectionMatch = "";
	string collectionFind = "";
	string collectionSize = "";
	string collectionConfidence = "";
	string collectionQualityModel = "";
	std::vector<string> tracerProcess;
	void EnrollUserAndTemplates(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client);
	void ImportUsers(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client);
	void ControlEntry(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client);
	void EnrollUserVideo(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client);
	void BuildUserDatabase(std::tuple<char*, vector<unsigned char>, int*, string> modelImage,
		int client, int userId);
	void GetFaceTemplateUser(int idUser, int index);
	void MatchUsers(const unsigned char* templateIn, int sizeIn, int client);
	void ConcatenateModeAuto(const unsigned char* templateIn, int sizeIn, int client);
	void ConcatenateModeForce(const unsigned char* templateIn, int sizeIn, int client);
	void ObserverError();
	void WithDeduplication(const unsigned char* data, int sizeImage, int client);
	void WithoutDeduplication(const unsigned char* data, int sizeImage, int client);
	
	void WithoutTemplatesRegisterUserWithDeduplication(std::tuple<char*, vector<unsigned char>,
		int*, string> modelImage, int client);
	void WithoutTemplatesRegisterUserWithoutDeduplication(std::tuple<char*, vector<unsigned char>,
		int*, string> modelImage, int client);
	void FinishRegisterUserAndTemplates();
	
	string BuildTracerString();
	bool CheckUserUnidentified(const unsigned char* data, int size);
	std::vector<string> TransformLogToVector(string content, string delimiter);
	string BuildJSONLog(string prevContent);
	void JoinedImageDetailVideo(string prevContent);
	string BuildEndTracerPrevVideo(string prevContent);
	string CropImageToStringBase64(vector<unsigned char> image, int rows, int cols);
	string BuildJSONCropImage(vector<std::string> values);
	void BuildTemplateForSend(vector<unsigned char> image, int rows, int cols, int client);
	int countAddFaceTemplate = 0;	
	bool connectionIdentification = false;
	int lastId = 1;
	bool isFinishLoadFiles = true;
	int countModelSendVideo = 0;
	User* userForDatabase = nullptr;
	vector<unsigned char> templateGuide;	
	CropTemplate* cropImage = new CropTemplate();	
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution{ 1, 1000};
	Base64* base64 = new Base64();
};


#endif // !FACEIDENTIFY_H

