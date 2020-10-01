#include "AipuAPI.h"
#include "Innovatrics.h"
#include "Pipe.h"


Innovatrics* innovatrics;
Database databaseMongo;

FrameView windowOGL;

vector<Pipe*> pipelines;
std::vector<std::thread> threadPipes;

string userJson;
string messageError;
bool isOpenWindow = false;
std::vector<string> listUser;
const string nameConfiguration =  "configuration";
const string fileDatabaseConfiguration = "database.txt";
const string fileGlobalConfiguration = "global.txt";


const char* fileConfigurations[4] = { "configPipeOne.txt", "configPipeTwo.txt", 
"configPipeThree.txt", "configPipeFour.txt" };


void AipuAPI::SetNumberPipelines(int value) {
	pipelines.clear();
	for (int i = 0; i < value; i++)
	{
		Pipe* pipe = new Pipe();
		pipelines.push_back(pipe);
	}
	ObserverError();
	pipelines[0]->SetDatabase(&databaseMongo);
}

void ThreadRunWindow() {
	int argc = 1;

	char appName[] = "AipuAPI.dll";
	char* arg[] = { appName, NULL };
	char** argv = { arg };
		
	windowOGL.RunOne(argc, argv);
}

void ThreadRunWindowTwo() {
	int argc = 1;

	char appName[] = "AipuAPI.dll";
	char* arg[] = { appName, NULL };
	char** argv = { arg };
	
	windowOGL.RunTwo(argc, argv);
}

void ThreadRunWindowFour() {
	int argc = 1;

	char appName[] = "AipuAPI.dll";
	char* arg[] = { appName, NULL };
	char** argv = { arg };
	
	windowOGL.RunFour(argc, argv);
}

void InitWindow() {
	std::thread togl(ThreadRunWindow);
	togl.detach();
	
	pipelines[0]->SetFrameView(&windowOGL);
}

void InitWindowTwo() {
	std::thread togl(ThreadRunWindowTwo);
	togl.detach();
	
	pipelines[0]->SetFrameView(&windowOGL);
}

void InitWindowFour() {
	std::thread togl(ThreadRunWindowFour);
	togl.detach();
	
	pipelines[0]->SetFrameView(&windowOGL);
}

void LoadOnlyOnePipe(int index) {
	pipelines[index]->SetNameDirectoryConfiguration(nameConfiguration);
	pipelines[index]->SetNameFileConfiguration(fileConfigurations[index]);
	pipelines[index]->SetClient(index + 1);
	pipelines[index]->SetIndexFrame(index + 1);
	pipelines[index]->LoadConfiguration();
}


void AipuAPI::LoadConfiguration(int option) {

	for (int i = 0; i < option; i++)
	{
		if (!pipelines[i]->GetIsLoadConfig())
		{
			LoadOnlyOnePipe(i);
		}
	}	

}

void AipuAPI::LoadConfigurationPipe(int pipeline) {
	int index = pipeline - 1;

	if (!pipelines[index]->GetIsLoadConfig())
	{
		LoadOnlyOnePipe(index);
	}

}

void AipuAPI::InitWindowMain(int option) {
	switch (option)
	{
	case 1:
		
		InitWindow();
		break;
	case 2:
		
		InitWindowTwo();
		break;
	case 3:
		
		InitWindowFour();
		break;
	case 4:
		
		InitWindowFour();
		break;
	default:
		break;
	}
	isOpenWindow = true;
}

void InitDatabaseMongo() {
	databaseMongo.configuration->SetNameDirectory(nameConfiguration);
	databaseMongo.configuration->SetNameFileConfiguration(fileDatabaseConfiguration);
	databaseMongo.configuration->ParseJSONToObject();
	databaseMongo.Configure();
	databaseMongo.CheckDatabase();
	databaseMongo.DeleteRecordsUserUnidentified();
	databaseMongo.DeleteRecordsImagesUnidentified();
}

AipuAPI::AipuAPI()
{
	innovatrics = new Innovatrics();	
	innovatrics->SetNameDirectory(nameConfiguration);
	innovatrics->SetNameFileConfiguration(fileGlobalConfiguration);
	innovatrics->ParseJSONToObject();
}

AipuAPI::~AipuAPI()
{
	delete innovatrics;
	pipelines.clear();
	threadPipes.clear();
}

void AipuAPI::ConnectDatabase() {
	InitDatabaseMongo();
	ObserverDatabase();

}

void AipuAPI::ObserverDatabase() {
	auto databaseError = databaseMongo.observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionDatabaseError = databaseError.subscribe([this](Either* either) {
		if (either->GetLabel() != "OK")
		{
			string message = to_string(either->GetCode()) + ": " + either->GetLabel();
			messageError = message;
			cout << messageError << endl;
		}
	});	

	auto databaseObservable = databaseMongo.observableUserJSON.map([](string jsonUser) {
		return jsonUser;
		});

	auto subscriptionDatabase = databaseObservable.subscribe([this](string jsonUser) {

		userJson = jsonUser;
		listUser.push_back(userJson);

		cout << userJson << endl;
	});

	subscriptionDatabase.clear();

}

void ExecuteFlowVideo(int index) {

	pipelines[index]->RunFlowVideo();

}

void AipuAPI::RunVideo(int option) {

	for (int i = 0; i < option; i++)
	{
		threadPipes.push_back(std::thread(ExecuteFlowVideo, i));
		threadPipes[i].detach();
	}

	
}

void AipuAPI::ReRunVideo(int option) {
	int index = option - 1;
	auto it = threadPipes.begin();
	threadPipes.insert(it + index, std::thread(ExecuteFlowVideo, index));
	threadPipes[index].detach();

}

void AipuAPI::CloseWindow() {

	for (int i = 0; i < pipelines.size(); i++)
	{
		pipelines[i]->SetFinishLoop();
	}
}

void AipuAPI::ObserverError() {

	vector<rxcpp::observable<Either*>> observers;
	for (int i = 0; i < pipelines.size(); i++)
	{
		observers.push_back(pipelines[i]->observableError.map([](Either* either) {
			return either;
			}));
	}


	vector<rxcpp::subscription> subscriptions;
	for (int i = 0; i < pipelines.size(); i++)
	{
		subscriptions.push_back(observers[i].subscribe([this](Either* either) {
			if (either->GetLabel() != "OK")
			{
				string message = to_string(either->GetCode()) + ": " + either->GetLabel();
				messageError = message;
				cout << messageError << endl;
			}
			}));
		
	}
	
}

void AipuAPI::Terminate() {

	for (int i = 0; i < pipelines.size(); i++)
	{
		if (pipelines[i]->GetIsLoadConfig())
		{
			pipelines[i]->RemoveUnidentified();
			pipelines[i]->CloseConnection();
		}
	}	

	listUser.clear();

	innovatrics->Terminate();

	if (isOpenWindow)
	{
		
		windowOGL.DestroyWindow();
		
	}
	
	
}

void AipuAPI::SetColourTextFrameOne(float red, float green, float blue) {
	windowOGL.SetColourTextFrameOne(red, green, blue);
}

void AipuAPI::SetColourTextFrameTwo(float red, float green, float blue) {
	windowOGL.SetColourTextFrameTwo(red, green, blue);
}

void AipuAPI::SetColourTextFrameThree(float red, float green, float blue) {
	windowOGL.SetColourTextFrameThree(red, green, blue);
}

void AipuAPI::SetColourTextFrameFour(float red, float green, float blue) {
	windowOGL.SetColourTextFrameFour(red, green, blue);
}

void AipuAPI::InitLibrary()
{
	auto observerErrorInnovatrics = innovatrics->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionErrorInnovatrics = observerErrorInnovatrics.subscribe([this](Either* either) {
		messageError = to_string(either->GetCode()) + ": " + either->GetLabel();

	});

	innovatrics->SetParamsLibrary();
	innovatrics->InitLibrary();
}


void AipuAPI::ReloadRecognitionFace() {
	innovatrics->SetParamsLibrary();
	innovatrics->InitLibrary();
	
}


string AipuAPI::GetUserJSON() {	

	string user = "";

	if (listUser.size() != 0)
	{
		user = *listUser.begin();
		cout << user << endl;
		listUser.erase(listUser.begin());
	}

	return user;
}

string AipuAPI::GetMessageError() {
	return messageError;
}


void AipuAPI::RecognitionFaceFiles(string file, int client, int task) {
	int index = client - 1;
	pipelines[index]->RecognitionFaceFiles(file, client, task);
	
}


void AipuAPI::SetIsFinishLoadFiles(int option, bool value) {

	int index = option - 1;
	pipelines[index]->SetIsFinishLoadFiles(value);
	
}

bool AipuAPI::GetIsFinishLoadFiles(int option) {
	int index = option - 1;
	return pipelines[index]->GetIsFinishLoadFiles();
	
}

void AipuAPI::ResetPerformance(int option) {
	int index = option - 1;
	pipelines[index]->ResetPerformance();

}

void AipuAPI::SavePerformance(int option) {
	int index = option - 1;
	pipelines[index]->SavePerformance();
	
}


void AipuAPI::StatePlay(int option) {
	int index = option - 1;
	pipelines[index]->StatePlay();
	
}

void AipuAPI::StatePaused(int option) {
	int index = option - 1;
	pipelines[index]->StatePaused();

}

void AipuAPI::SetFinishLoop(int option) {
	int index = option - 1;
	pipelines[index]->SetFinishLoop();

}

void AipuAPI::SetTaskIdentify(int value, int option) {
	int index = option - 1;
	pipelines[index]->SetTaskIdentify(value);	
}

void AipuAPI::ResetEnrollVideo(int option) {
	int index = option - 1;
	pipelines[index]->ResetEnrollVideo();
	
}

void AipuAPI::AddCollectionOfImages(string folder, int client, int doing) {
	int index = client - 1;
	pipelines[index]->AddCollectionOfImages(folder, client, doing);

}