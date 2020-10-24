#ifndef FACEIDENTIFY_H
#define FACEIDENTIFY_H

#include "ErrorIdKitLib.h"
#include "Molded.h"
#include "User.h"
#include "Format.h"
#include "FaceIdkit.h"
#include <random>


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

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
	Rx::subject<User*> userSubject;
	Rx::observable<User*> observableUser = userSubject.get_observable();
private:

	ErrorIdKitLib* error = new ErrorIdKitLib();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	Rx::subscriber<User*> shootUser = userSubject.get_subscriber();
	bool flagEnroll = false;
	Format* format = new Format();
	FaceIdkit* faceIdkit = new FaceIdkit();
	File* file = new File();
	Configuration* turnIntoLogToJSON = new Configuration();
	string tracerImage;
	string tracerPrevImage = "";
	string tracerMatch = "";	
	std::vector<string> tracerProcess;
	void EnrollUserAndTemplates(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client);
	void ImportUsers(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client);
	void ControlEntry(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client);
	void EnrollUserVideo(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client);
	void BuildUserDatabase(std::tuple<char*, vector<unsigned char>, int*, string> modelImage,
		int client, int userId);
	void GetFaceTemplateUser(int idUser, int index);
	void MatchUsers(const unsigned char* templateIn, int sizeIn);
	void ConcatenateModeAuto(const unsigned char* templateIn, int sizeIn);
	void ConcatenateModeForce(const unsigned char* templateIn, int sizeIn);
	void ObserverError();
	void WithDeduplication(const unsigned char* data, int sizeImage);
	void WithoutDeduplication(const unsigned char* data, int sizeImage);
	
	void WithoutTemplatesRegisterUserWithDeduplication(std::tuple<char*, vector<unsigned char>,
		int*, string> modelImage, int client);
	void WithoutTemplatesRegisterUserWithoutDeduplication(std::tuple<char*, vector<unsigned char>,
		int*, string> modelImage, int client);
	void FinishRegisterUserAndTemplates();
	
	string BuildTracerString();
	bool CheckUserUnidentified(const unsigned char* data, int size);
	std::vector<string> TransformLogToVector(string content, string delimiter);
	string BuildJSONLog(string prevContent);
	int countAddFaceTemplate = 0;
		
	int lastId = 1;
		
	int countModelSendVideo = 0;
	User* userForDatabase = nullptr;
	vector<unsigned char> templateGuide;		
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution{ 1, 1000};
	
};


#endif // !FACEIDENTIFY_H

