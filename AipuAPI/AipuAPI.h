#pragma once
#ifdef AIPUAPI_EXPORTS
#define AIPUAPI_API __declspec(dllexport)
#else
#define AIPUAPI_API __declspec(dllimport)
#endif

#include <string>


using namespace std;

class AIPUAPI_API AipuAPI
{
public:
	AipuAPI();
	~AipuAPI();
	void ConnectDatabase();
	void InitLibrary();	
	void LoadConfiguration(int option); // 1 2 3 4 throw group
	void InitWindowMain(int option); // 1 2 3 4 throw group
	void RunVideo(int option); // 1 2 3 4 throw group
	void LoadConfigurationPipe(int pipeline); // 1 2 3 4 throw single
	void CloseWindow();
	void Terminate();
	void ReRunVideo(int option); // throw single
	void SetColourTextFrameOne(float red, float green, float blue);
	void SetColourTextFrameTwo(float red, float green, float blue);
	void SetColourTextFrameThree(float red, float green, float blue);
	void SetColourTextFrameFour(float red, float green, float blue);
	void ReloadRecognitionFace();
	void AddCollectionOfImages(string folder, int client, int doing);
	void RecognitionFaceFiles(string file, int client, int task);
	void SetIsFinishLoadFiles(int option, bool value);
	bool GetIsFinishLoadFiles(int option);	
	void SetTaskIdentify(int value, int option);
	void ResetEnrollVideo(int option, int value);
	void AddUserEnrollVideo(int channel);
	void StatePlay(int option); // single
	void StatePaused(int option); // single
	void SetFinishLoop(int option); // single
	string GetUserJSON();
	string GetMessageError();
	

	void SetNumberPipelines(int value);
	
private:
	void ObserverError();
	void ObserverDatabase();
	
};
