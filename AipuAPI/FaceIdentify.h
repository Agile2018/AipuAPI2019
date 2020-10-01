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
	void ForkTask(std::tuple<char*, vector<unsigned char>, int*> modelImage, int client);
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

	void ResetEnrollVideo() {
		
		countModelSendVideo = 0;
	}


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
	
	void EnrollUserAndTemplates(std::tuple<char*, vector<unsigned char>, int*> modelImage, int client);
	void ImportUsers(std::tuple<char*, vector<unsigned char>, int*> modelImage, int client);
	void ControlEntry(std::tuple<char*, vector<unsigned char>, int*> modelImage, int client);	
	void EnrollUserVideo(std::tuple<char*, vector<unsigned char>, int*> modelImage, int client);
	void BuildUserDatabase(std::tuple<char*, vector<unsigned char>, int*> modelImage, 
		int client, int userId);
	void GetFaceTemplateUser();
	void MatchUsers(const unsigned char* templateIn, int sizeIn);
	void ObserverError();
	int countRepeatUser = 0;
	int countNewUser = 0;
	int lastUserUnidentified = 0;	
	int countRepeatFrame = 0;	
	int lastId = 0;
	const unsigned char* templateForMatch = NULL;
	int lenghtMatch = 0;	
	int countModelSendVideo = 0;
};


#endif // !FACEIDENTIFY_H

