#ifndef User_h
#define User_h

#include <iostream>
#include <vector>

using namespace std;
class User
{
public:
	User();
	~User();
	const string fieldUserIdFace = "id_face";
	const string fieldNameUser = "name";
	const string fieldAddressUser = "address";
	const string fieldId = "_id";
	const string fieldClient = "client";

	void SetIdDb(string id) {
		idDb = id;
	}
	void SetUserIdIFace(int id) {
		userIdIFace = id;
	}

	void SetNameUser(string name) {
		nameUser = name;
	}

	void SetLastNameUser(string lastName) {
		lastNameUser = lastName;
	}
	string GetIdDb() {
		return idDb;
	}
	int GetUserIdIFace() {
		return userIdIFace;
	}

	string GetNameUser() {
		return nameUser;
	}

	string GetLastNameUser() {
		return lastNameUser;
	}

	void SetLogProcess(string log) {
		logProcess = log;
	}

	string GetLogProcess() {
		return logProcess;
	}

	void SetStateUser(int value) {
		state = value;
	}

	int GetStateUser() {
		return state;
	}

	void SetClient(int id) {
		client = id;
	}
	int GetClient() {
		return client;
	}

	void SetIdentificationUser(string identification) {
		identificationUser = identification;
	}

	string GetIdentificationUser() {
		return identificationUser;
	}

	int GetMoldCropWidth() {
		return cropWidth;
	}

	int GetMoldCropHeight() {
		return cropHeight;
	}

	int GetMoldScore() {
		return score;
	}

	void SetCropImageData(vector<unsigned char> moldCrop) {
		copy(moldCrop.begin(), moldCrop.end(), back_inserter(cropImageData));

	}

	vector<unsigned char> GetCropImageData() {
		return cropImageData;
	}

	void SetMoldCropWidth(int size) {
		cropWidth = size;
	}

	void SetMoldCropHeight(int size) {
		cropHeight = size;
	}

	void SetMoldScore(int value) {
		score = value;
	}

private:
	string idDb;
	int userIdIFace = -1, cropWidth = -1, cropHeight = -1, score = -1;
	string nameUser;
	string lastNameUser;
	string identificationUser;
	string logProcess;
	int state = 2; // 1 -> New, 2 -> enroll, 3 -> nothing
	int client = 0;

	vector<unsigned char> cropImageData;

};


#endif // !User_h

