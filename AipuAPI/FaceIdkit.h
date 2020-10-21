#ifndef FaceIdkit_h
#define FaceIdkit_h

#include "ErrorIdKitLib.h"
#include "ConfigurationIdentify.h"


class FaceIdkit
{
public:
	FaceIdkit();
	~FaceIdkit();
	static int InitLibrary();
	static int TerminateLibrary();	
	void Connection();
	int FindUser(const unsigned char* templateModel, int size, int* userId, int* score, string* tracer);
	int RegisterUser(const unsigned char* templateModel, int size, int* userId);	
	int RegisterUserWithData(const unsigned char* templateModel, int size,
		const unsigned char* userData, int dataLength, int* userId);
	int RemoveUser(int userId);	
	int MatchUsers(const unsigned char* templateModelOne,
		int sizeOne, const unsigned char* templateModelTwo, int sizeTwo, int* score);
	int AddFaceTemplate(const unsigned char* templateModel, int size, int userID);
	int AddFaceTemplate(const unsigned char* templateModel, int size);
	void CloseConnection();	
	unsigned char* GetFaceTemplate(int userId, int* lenght, int index);
	void SetStringJSON(string stringJson) {
		configuration->SetStringJSON(stringJson);
	}
	int GetSimilarityThreshold();
	void ClearUser();
	ConfigurationIdentify* configuration = new ConfigurationIdentify();
	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
private:
	ErrorIdKitLib* error = new ErrorIdKitLib();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	
	IENGINE_CONNECTION handleConnect = nullptr;
	IENGINE_USER user = nullptr;
	IENGINE_USER userAux = nullptr;
	IENGINE_USER userAuxMatch = nullptr;
	void SetParameters();
	
};


#endif // !FaceIdkit_h

