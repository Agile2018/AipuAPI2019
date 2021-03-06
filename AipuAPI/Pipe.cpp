#include "Pipe.h"

Database* database;


Pipe::Pipe()
{	
	ObserverTemplateImage();
	ObserverIdentifyFace();	
	ObserverError();
	ObserverTrackingFace();
	ObserverCropImage();
}

Pipe::~Pipe()
{
	delete faceModel;
	delete faceIdentify;	
	delete configurationFile;	
	delete flowVideo;
}

void Pipe::SetFrameView(FrameView* frame) {
	flowVideo->SetFrameView(frame);
}

void Pipe::SetDatabase(Database* db) {
	database = db;
}

void Pipe::LoadConfiguration() {
	configurationFile->SetNameDirectory(directoryConfiguration);
	configurationFile->SetNameFileConfiguration(fileConfiguration);
	configurationFile->ParseJSONToObject();
	
	flowVideo->configuration->SetStringJSON(configurationFile->GetStringJSON());
	flowVideo->configuration->ParseJSONToObject();
	
	faceModel->SetStringJSON(configurationFile->GetStringJSON());
	faceModel->ParseJSONToObject();
	
	faceModel->InitHandle();
	
	flowVideo->SetStringJSONTracking(configurationFile->GetStringJSON());
	flowVideo->SetConfigurationIFace(faceModel->configuration);
	
	faceIdentify->SetStringJSON(configurationFile->GetStringJSON());	
	faceIdentify->ParseJSONToObject();
	faceIdentify->LoadConnection();		

	isLoadConfig = true;
}

void Pipe::CloseConnectionIdentification() {
	if (faceIdentify->GetConnectionIdentification())
	{
		faceIdentify->CloseConnection();
	}
	
}

void Pipe::LoadConnectionIdentification() {
	if (!faceIdentify->GetConnectionIdentification() && GetIsLoadConfig()) {
		faceIdentify->LoadConnection();
	}
	
}

int Pipe::GetTaskIdentify() {
	return flowVideo->GetTaskIdentify();
}

void Pipe::RunFlowVideo() {
	flowVideo->RunFlowVideo();
}

void Pipe::RecognitionFaceFiles(string namefile, int client, int task) {
	faceModel->RecognitionFaceFiles(namefile, client, task);
}

void Pipe::SetIsFinishLoadFiles(bool value) {
	faceModel->SetIsFinishLoadFiles(value);
	faceIdentify->SetIsFinishLoadFiles(value);
}

bool Pipe::GetIsFinishLoadFiles() {
	
	return faceModel->GetIsFinishLoadFiles() || faceIdentify->GetIsFinishLoadFiles();
}

void Pipe::StatePlay() {
	flowVideo->StatePlay();
}

void Pipe::StatePaused() {
	flowVideo->StatePaused();
}

void Pipe::SetFinishLoop() {
	flowVideo->SetFinishLoop();
}

void Pipe::SetTaskIdentify(int value) {
	flowVideo->SetTaskIdentify(value);
}

void Pipe::SetIndexFrame(int value)
{
	flowVideo->SetIndexFrame(value);
}

void Pipe::ObserverCropImage() {
	auto cropImage = faceIdentify->observableCropImage.map([](string dataImage) {
		return dataImage;
	});
	auto subscriptionCropImage = cropImage.subscribe([this](string dataImage) {
		shootCropImage.on_next(dataImage);
		});
}

void Pipe::ObserverError() {
	auto faceModelError = faceModel->observableError.map([](Either* either) {
		return either;
		});

	auto subscriptionFaceModelError = faceModelError.subscribe([this](Either* either) {
		if (either->GetLabel() != "OK")
		{
			shootError.on_next(either);
		}

		});

	auto configurationFileError = configurationFile->observableError.map([](Either* either) {
		return either;
		});

	auto subscriptionConfigurationFileError = configurationFileError.subscribe([this](Either* either) {
		shootError.on_next(either);
		});

	auto identifyError = faceIdentify->observableError.map([](Either* either) {
		return either;
		});

	auto subscriptionIdentifyError = identifyError.subscribe([this](Either* either) {
		shootError.on_next(either);
		});
	

	auto flowVideoError = flowVideo->observableError.map([](Either* either) {
		return either;
		});

	auto subscriptionFlowVideoError = flowVideoError.subscribe([this](Either* either) {
		shootError.on_next(either);
		});

}

void Pipe::ObserverTrackingFace()
{
	auto templateObservable = flowVideo->observableTrackedFace
		.map([](std::tuple<char*, vector<unsigned char>, int*, string> modelImage) {
		return modelImage;
	});

	auto subscriptionTemplate = templateObservable
		.subscribe([this](std::tuple<char*, vector<unsigned char>, int*, string> modelImage) {
		
		faceIdentify->ForkTask(modelImage, client);
		
		
	});

	subscriptionTemplate.clear();
}

void Pipe::ObserverTemplateImage()
{
	auto templateObservable = faceModel->observableTemplate.map([](std::tuple<char*, 
		vector<unsigned char>, int*, string> modelImage) {
		return modelImage;
		});

	auto subscriptionTemplate = templateObservable.subscribe([this](std::tuple<char*, 
		vector<unsigned char>, int*, string> modelImage) {
		
		faceIdentify->ForkTask(modelImage, client);
		
	});

	subscriptionTemplate.clear();
}

void Pipe::ObserverIdentifyFace() {
	auto identifyUser = faceIdentify->observableUser.map([](User* user) {
		return user;
	});

	auto subscriptionIdentifyUser = identifyUser.subscribe([this](User* user) {
		
		database->ProcessUserDB(user);
		delete user;
	});

	subscriptionIdentifyUser.clear();
}


void Pipe::AddCollectionOfImages(string folder, int client, int doing) {
	faceModel->AddCollectionOfImages(folder, client, doing);
}

void Pipe::ResetEnrollVideo(int value) {
	faceIdentify->ResetEnrollVideo(value);
}

void Pipe::AddUserEnrollVideo() {
	faceIdentify->AddUserEnrollVideo();
}

void Pipe::DownConfigurationModel() {
	if (GetIsLoadConfig()) {
		faceModel->TerminateHandle();
	}
	
}

void Pipe::LoadConfigurationModel() {
	if (GetIsLoadConfig()) {
		configurationFile->SetNameDirectory(directoryConfiguration);
		configurationFile->SetNameFileConfiguration(fileConfiguration);
		configurationFile->ParseJSONToObject();
		faceModel->SetStringJSON(configurationFile->GetStringJSON());
		faceModel->ParseJSONToObject();
		faceModel->InitHandle();
	}

}

void Pipe::LoadConfigurationIdentify() {
	if (GetIsLoadConfig()) {
		configurationFile->SetNameDirectory(directoryConfiguration);
		configurationFile->SetNameFileConfiguration(fileConfiguration);
		configurationFile->ParseJSONToObject();
		faceIdentify->SetStringJSON(configurationFile->GetStringJSON());
		faceIdentify->ParseJSONToObject();
		faceIdentify->LoadConnection();
	}
}

void Pipe::LoadConfigurationTracking() {
	if (GetIsLoadConfig()) {
		configurationFile->SetNameDirectory(directoryConfiguration);
		configurationFile->SetNameFileConfiguration(fileConfiguration);
		configurationFile->ParseJSONToObject();
		flowVideo->SetStringJSONTracking(configurationFile->GetStringJSON());
		flowVideo->SetConfigurationIFace(faceModel->configuration);
	}
}