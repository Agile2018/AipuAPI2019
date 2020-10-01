#ifndef Pipe_h
#define Pipe_h

#include <iostream>
#include "ErrorAipuLib.h"
#include "FlowVideo.h"
#include "ConfigurationFile.h"
#include "FaceModel.h"
#include "FaceIdentify.h"
#include "Database.h"

using namespace std;

class Pipe
{
public:
	Pipe();
	~Pipe();
	void LoadConfiguration();
	//void SetDirectoryConfiguration();
	void RunFlowVideo();
	void StatePlay();
	void StatePaused();
	void SetFinishLoop();
	void SetNameDirectoryConfiguration(string directory) {
		directoryConfiguration = directory;
	}
	void SetNameFileConfiguration(string file) {
		fileConfiguration = file;
	}

	void SetClient(int value) {
		client = value;
	}
	void SetFrameView(FrameView* frame);
	void SetIndexFrame(int value);
	void SetDatabase(Database* db);
	void RecognitionFaceFiles(string file, int client, int task);
	void SetIsFinishLoadFiles(bool value);
	bool GetIsFinishLoadFiles();
	void ResetLowScore();
	int GetCountLowScore();
	void ResetCountNotDetect();
	int GetCountNotDetect();
	void ResetCountRepeatUser();
	int GetCountRepeatUser();
	void SavePerformance();
	void ResetPerformance();
	bool GetIsLoadConfig() {
		return isLoadConfig;
	}
	void SetTaskIdentify(int value);
	void ResetEnrollVideo();
	void RemoveUnidentified();
	void CloseConnection();
	void AddCollectionOfImages(string folder, int client, int doing);
	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
	/*Rx::subject<string> userDetected;
	Rx::observable<string> observableUserJSON = userDetected.get_observable();*/
private:
	string fileConfiguration = "";
	const string filePerformance = "performance.txt";
	string directoryConfiguration = "configuration";
	int client = 1;
	bool isLoadConfig = false;
	ConfigurationFile* configurationFile = new ConfigurationFile();
	Configuration* configurationPerformance = new Configuration();
	FaceModel* faceModel = new FaceModel();
	FaceIndentify* faceIdentify = new FaceIndentify();
	FlowVideo* flowVideo = new FlowVideo();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	//Rx::subscriber<string> shootUserJSON = userDetected.get_subscriber();
	void ObserverError();
	void ObserverTemplateImage();
	void ObserverIdentifyFace();
	//void ObserverDatabase();
	void ObserverTrackingFace();	
	//void SetNameFileConfigurationFace(string name);
	//void SetNameFileConfigurationIdentify(string name);
	//void SetNameFileConfigurationDatabase(string name);
	//void SetNameFileConfigurationTracking(string name);
	//void SetNameFileConfigurationFlowVideo(string name);
};

#endif // !Pipe_h

