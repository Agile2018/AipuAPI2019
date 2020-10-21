#ifndef FACEIDENTIFY_H
#define FACEIDENTIFY_H

#include "ErrorIdKitLib.h"
#include "Molded.h"
#include "User.h"
#include "Format.h"
#include "FaceIdkit.h"

class FaceIndentify
{
public:
	FaceIndentify();
	~FaceIndentify();
	void LoadConnection();	
	void RemoveUnidentified();
	void CloseConnection();	
	void ForkTask(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client);
	bool GetFlagEnroll() {
		return flagEnroll;
	}

	void ResetCountRepeatUser() {
		countRepeatUser = 0;
	}

	int GetCountRepeatUser() {
		return countRepeatUser;
	}

	void ResetCountNewUser() {
		countNewUser = 0;
	}

	int GetCountNewUser() {
		return countNewUser;
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
	string tracerImage;
	string tracerPrevImage = "";
	string tracerMatch = "";
	string tracerHeadImage = "";
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
	void WriteHeadLog();
	//void WriteHeadLog(string tracer);
	void WithoutTemplatesRegisterUserWithDeduplication(std::tuple<char*, vector<unsigned char>,
		int*, string> modelImage, int client);
	void WithoutTemplatesRegisterUserWithoutDeduplication(std::tuple<char*, vector<unsigned char>,
		int*, string> modelImage, int client);
	void FinishRegisterUserAndTemplates();
	void BuildTracerPrevious(string previous);
	string BuildTracerString();
	int countRepeatUser = 0;
	int countNewUser = 0;
	int lastUserUnidentified = 0;	
	int countAddFaceTemplate = 0;
	//int countRepeatFrame = 0;	
	int lastId = 1;
	//const unsigned char* templateForMatch = NULL;
	//int lenghtMatch = 0;	
	int countModelSendVideo = 0;
	User* userForDatabase = nullptr;
	vector<unsigned char> templateGuide;
};


#endif // !FACEIDENTIFY_H

