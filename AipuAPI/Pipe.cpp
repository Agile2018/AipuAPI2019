#include "Pipe.h"

Database* database;


Pipe::Pipe()
{	
	ObserverTemplateImage();
	ObserverIdentifyFace();	
	ObserverError();
	ObserverTrackingFace();
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

void Pipe::SetDirectoryConfiguration() {
	faceModel->configuration->SetNameDirectory(directoryConfiguration);
	faceIdentify->configuration->SetNameDirectory(directoryConfiguration);
	configurationFile->SetNameDirectory(directoryConfiguration);	
	flowVideo->SetNameDirectoryTracking(directoryConfiguration);
	flowVideo->configuration->SetNameDirectory(directoryConfiguration);
	configurationPerformance->SetNameDirectory(directoryConfiguration);
}

void Pipe::ResetPerformance() {
	faceModel->ResetLowScore();
	faceModel->ResetCountNotDetect();
	faceModel->ResetCountErrorDetect();
	faceModel->ResetCountDetect();
	faceIdentify->ResetCountRepeatUser();
	faceIdentify->ResetCountNewUser();
}

void Pipe::SavePerformance() {
	configurationPerformance->jsonBody.clear();
	configurationPerformance->jsonParams.clear();
	std::map<std::string, std::int16_t> params;

	params.insert(std::pair<std::string, std::int16_t>("LowScore", faceModel->GetCountLowScore()));
	params.insert(std::pair<std::string, std::int16_t>("NotDetect", faceModel->GetCountNotDetect()));
	params.insert(std::pair<std::string, std::int16_t>("ErrorDetect", faceModel->GetCountErrorDetect()));
	params.insert(std::pair<std::string, std::int16_t>("Detect", faceModel->GetCountDetect()));
	params.insert(std::pair<std::string, std::int16_t>("RepeatUser", faceIdentify->GetCountRepeatUser()));
	params.insert(std::pair<std::string, std::int16_t>("NewUser", faceIdentify->GetCountNewUser()));
	
	std::map<std::string, std::int16_t>::const_iterator it = params.begin(),
		end = params.end();

	for (; it != end; ++it) {
		configurationPerformance->jsonParams[it->first] = it->second;

	}

	configurationPerformance->jsonBody["configuration"] = "performance_configuration";
	configurationPerformance->jsonBody["Params"] = configurationPerformance->jsonParams;

	configurationPerformance->SaveConfiguration();
}

void Pipe::LoadConfiguration() {
	configurationFile->SetNameFileConfiguration(fileConfiguration);
	configurationFile->ParseJSONToObject();
	SetNameFileConfigurationFace(configurationFile->GetNameFileConfigurationFaceModel());
	SetNameFileConfigurationIdentify(configurationFile->GetNameFileConfigurationIdentify());		
	SetNameFileConfigurationTracking(configurationFile->GetNameFileConfigurationTracking());
	SetNameFileConfigurationFlowVideo(configurationFile->GetNameFileConfigurationFlow());
	configurationPerformance->SetNameFileConfiguration(filePerformance);
}

void Pipe::RunFlowVideo() {
	flowVideo->RunFlowVideo();
}

void Pipe::RecognitionFaceFiles(string file, int client) {
	faceModel->RecognitionFaceFiles(file, client);
}

void Pipe::SetIsFinishLoadFiles(bool value) {
	faceModel->SetIsFinishLoadFiles(value);
}

bool Pipe::GetIsFinishLoadFiles() {
	return faceModel->GetIsFinishLoadFiles();
}

void Pipe::ResetLowScore() {
	faceModel->ResetLowScore();
}

int Pipe::GetCountLowScore() {
	return faceModel->GetCountLowScore();
}
void Pipe::ResetCountNotDetect() {
	faceModel->ResetCountNotDetect();
}

int Pipe::GetCountNotDetect() {
	return faceModel->GetCountNotDetect();
}

void Pipe::ResetCountRepeatUser() {
	faceIdentify->ResetCountRepeatUser();
}

int Pipe::GetCountRepeatUser() {
	return faceIdentify->GetCountRepeatUser();
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

void Pipe::SetNameFileConfigurationFace(string name) {
	faceModel->configuration->SetNameFileConfiguration(name);
	faceModel->configuration->ParseJSONToObject();
	faceModel->InitHandle();
}

void Pipe::SetNameFileConfigurationIdentify(string name) {
	faceIdentify->configuration->SetNameFileConfiguration(name);
	faceIdentify->configuration->ParseJSONToObject();
	faceIdentify->LoadConnection();
}


void Pipe::SetNameFileConfigurationTracking(string name) {	
	flowVideo->SetNameFileConfigurationTracking(name);
}

void Pipe::SetNameFileConfigurationFlowVideo(string name) {
	flowVideo->configuration->SetNameFileConfiguration(name);
	flowVideo->configuration->ParseJSONToObject();
	//flowVideo->configuration->ParseMapToJSON();
	//cout << flowVideo->configuration->GetStringJSON().c_str() << endl;
}

void Pipe::SetIndexFrame(int value)
{
	flowVideo->SetIndexFrame(value);
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
		.map([](std::tuple<char*, vector<unsigned char>, int*> modelImage) {
		return modelImage;
	});

	auto subscriptionTemplate = templateObservable
		.subscribe([this](std::tuple<char*, vector<unsigned char>, int*> modelImage) {
		
		faceIdentify->EnrollUser(modelImage, client);
		
		
	});

	subscriptionTemplate.clear();
}

void Pipe::ObserverTemplateImage()
{
	auto templateObservable = faceModel->observableTemplate.map([](std::tuple<char*, 
		vector<unsigned char>, int*> modelImage) {
		return modelImage;
		});

	auto subscriptionTemplate = templateObservable.subscribe([this](std::tuple<char*, 
		vector<unsigned char>, int*> modelImage) {
		
		faceIdentify->EnrollUser(modelImage, client);
		
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

