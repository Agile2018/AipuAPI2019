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
	void RecognitionFaceFiles(string namefile, int client, int task);
	void SetIsFinishLoadFiles(bool value);
	bool GetIsFinishLoadFiles();
	
	bool GetIsLoadConfig() {
		return isLoadConfig;
	}

	void ResetConfiguration() {
		isLoadConfig = false;
	}

	void SetTaskIdentify(int value);
	void ResetEnrollVideo(int value);
	
	void CloseConnectionIdentification();
	void LoadConnectionIdentification();	
	void AddCollectionOfImages(string folder, int client, int doing);
	void AddUserEnrollVideo();
	void DownConfigurationModel();
	void LoadConfigurationModel();
	void LoadConfigurationIdentify();
	void LoadConfigurationTracking();
	int GetTaskIdentify();
	
	void SetIndexthread(int value) {
		indexthread = value;
	}

	int GetIndexthread() {
		return indexthread;
	}

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
	Rx::subject<string> cropImageSubject;
	Rx::observable<string> observableCropImage = cropImageSubject.get_observable();
private:
	string fileConfiguration = "";	
	string directoryConfiguration = "configuration";
	int client = 1;
	bool isLoadConfig = false;
	int indexthread = 0;
	ConfigurationFile* configurationFile = new ConfigurationFile();	
	FaceModel* faceModel = new FaceModel();
	FaceIndentify* faceIdentify = new FaceIndentify();
	FlowVideo* flowVideo = new FlowVideo();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	Rx::subscriber<string> shootCropImage = cropImageSubject.get_subscriber();
	void ObserverError();
	void ObserverTemplateImage();
	void ObserverIdentifyFace();
	void ObserverCropImage();
	void ObserverTrackingFace();	
	
};

#endif // !Pipe_h

