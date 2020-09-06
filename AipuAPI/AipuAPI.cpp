#include "AipuAPI.h"
#include "Innovatrics.h"
#include "Pipe.h"


Innovatrics* innovatrics;
Database databaseMongo;

FrameView windowOGL;
Pipe* pipe1;
Pipe* pipe2;
Pipe* pipe3;
Pipe* pipe4;
string userJson;
string messageError;
bool isOpenWindow = false;
std::vector<string> listUser;
const string nameConfigurationOne = "configuration";
const string nameConfigurationTwo = "configuration1";
const string nameConfigurationThree = "configuration2";
const string nameConfigurationFour = "configuration3";
const string fileDatabaseConfiguration = "database.txt";
//typedef std::unordered_map<std::string, pthread_t> ThreadMap;
//ThreadMap tm_;
const string nameThreadOne = "threadOne";
const string nameThreadTwo = "threadTwo";
const string nameThreadThree = "threadThree";
const string nameThreadFour = "threadFour";


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
	pipe1->SetFrameView(&windowOGL);
	
}

void InitWindowTwo() {
	std::thread togl(ThreadRunWindowTwo);
	togl.detach();
	pipe1->SetFrameView(&windowOGL);
	
}

void InitWindowFour() {
	std::thread togl(ThreadRunWindowFour);
	togl.detach();
	pipe1->SetFrameView(&windowOGL);
	
}

void LoadConfigurationVideoOne() {
	pipe1->SetNameDirectoryConfiguration(nameConfigurationOne);
	pipe1->SetDirectoryConfiguration();
	pipe1->SetClient(PIPE_ONE);
	pipe1->SetIndexFrame(PIPE_ONE);
	pipe1->LoadConfiguration();
	
}

void LoadConfigurationVideoTwo() {
	pipe2->SetNameDirectoryConfiguration(nameConfigurationTwo);
	pipe2->SetDirectoryConfiguration();
	pipe2->SetClient(PIPE_TWO);
	pipe2->SetIndexFrame(PIPE_TWO);
	pipe2->LoadConfiguration();
}

void LoadConfigurationVideoThree() {
	pipe3->SetNameDirectoryConfiguration(nameConfigurationThree);
	pipe3->SetDirectoryConfiguration();
	pipe3->SetClient(PIPE_THREE);
	pipe3->SetIndexFrame(PIPE_THREE);
	pipe3->LoadConfiguration();
}

void LoadConfigurationVideoFour() {
	pipe4->SetNameDirectoryConfiguration(nameConfigurationFour);
	pipe4->SetDirectoryConfiguration();
	pipe4->SetClient(PIPE_FOUR);
	pipe4->SetIndexFrame(PIPE_FOUR);
	pipe4->LoadConfiguration();
}

void AipuAPI::LoadConfiguration(int option) {
	switch (option)
	{
	case 1:
		if (!pipe1->GetIsLoadConfig())
		{
			LoadConfigurationVideoOne();
		}
				
		break;
	case 2:
		if (!pipe1->GetIsLoadConfig())
		{
			LoadConfigurationVideoOne();
		}
		if (!pipe2->GetIsLoadConfig())
		{
			LoadConfigurationVideoTwo();
		}
		
		
		break;
	case 3:
		if (!pipe1->GetIsLoadConfig())
		{
			LoadConfigurationVideoOne();
		}
		if (!pipe2->GetIsLoadConfig())
		{
			LoadConfigurationVideoTwo();
		}
		if (!pipe3->GetIsLoadConfig())
		{
			LoadConfigurationVideoThree();
		}
		
		
		break;
	case 4:
		if (!pipe1->GetIsLoadConfig())
		{
			LoadConfigurationVideoOne();
		}
		if (!pipe2->GetIsLoadConfig())
		{
			LoadConfigurationVideoTwo();
		}
		if (!pipe3->GetIsLoadConfig())
		{
			LoadConfigurationVideoThree();
		}
		if (!pipe4->GetIsLoadConfig())
		{
			LoadConfigurationVideoFour();
		}
			
		break;
	default:
		break;
	}
}

void AipuAPI::LoadConfigurationPipe(int pipeline) {
	switch (pipeline)
	{
	case 1:
		if (!pipe1->GetIsLoadConfig())
		{
			LoadConfigurationVideoOne();
		}

		break;
	case 2:
		
		if (!pipe2->GetIsLoadConfig())
		{
			LoadConfigurationVideoTwo();
		}

		break;
	case 3:
		
		if (!pipe3->GetIsLoadConfig())
		{
			LoadConfigurationVideoThree();
		}

		break;
	case 4:
		
		if (!pipe4->GetIsLoadConfig())
		{
			LoadConfigurationVideoFour();
		}

		break;
	default:
		break;
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
	databaseMongo.configuration->SetNameDirectory(nameConfigurationOne);
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
			
}

AipuAPI::~AipuAPI()
{
	delete innovatrics;
	delete pipe1;
	delete pipe2;
	delete pipe3;
	delete pipe4;
	
}

void AipuAPI::ConnectDatabase() {
	InitDatabaseMongo();
	ObserverDatabase();
	pipe1 = new Pipe();
	pipe2 = new Pipe();
	pipe3 = new Pipe();
	pipe4 = new Pipe();
	ObserverError();
	pipe1->SetDatabase(&databaseMongo);
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



void ExecuteFlowVideoOne() {	
					
	pipe1->RunFlowVideo();	
	
}



void ExecuteFlowVideoTwo() {	
	
	pipe2->RunFlowVideo();
	
}



void ExecuteFlowVideoThree() {	
	
	pipe3->RunFlowVideo();
	
}



void ExecuteFlowVideoFour() {	
	
	pipe4->RunFlowVideo();
	
}
void RunVideoOne() {		
	std::thread tpipe1(ExecuteFlowVideoOne);		
	tpipe1.detach();	
}

void RunVideoTwo() {
	std::thread tpipe1(ExecuteFlowVideoOne);
	std::thread tpipe2(ExecuteFlowVideoTwo);	
	tpipe1.detach();
	tpipe2.detach();

}

void RunVideoThree() {
	std::thread tpipe1(ExecuteFlowVideoOne);
	std::thread tpipe2(ExecuteFlowVideoTwo);
	std::thread tpipe3(ExecuteFlowVideoThree);	
	tpipe1.detach();
	tpipe2.detach();
	tpipe3.detach();
	
}

void RunVideoFour() {
	std::thread tpipe1(ExecuteFlowVideoOne);
	std::thread tpipe2(ExecuteFlowVideoTwo);
	std::thread tpipe3(ExecuteFlowVideoThree);
	std::thread tpipe4(ExecuteFlowVideoFour);
	tpipe1.detach();
	tpipe2.detach();
	tpipe3.detach();
	tpipe4.detach();
	
}

void ReRunVideoOne() {
	LoadConfigurationVideoOne();
	std::thread tpipe1(ExecuteFlowVideoOne);
	tpipe1.detach();
}

void ReRunVideoTwo() {
	std::thread tpipe2(ExecuteFlowVideoTwo);
	tpipe2.detach();
}

void ReRunVideoThree() {
	std::thread tpipe3(ExecuteFlowVideoThree);
	tpipe3.detach();
}

void ReRunVideoFour() {
	std::thread tpipe4(ExecuteFlowVideoFour);
	tpipe4.detach();
}

void AipuAPI::RunVideo(int option) {
	switch (option)
	{
	case 1:
		RunVideoOne();
		break;
	case 2:
		RunVideoTwo();
		break;
	case 3:
		RunVideoThree();
		break;
	case 4:
		RunVideoFour();
		break;
	default:
		break;
	}

	
}

void AipuAPI::ReRunVideo(int option) {
	switch (option)
	{
	case 1:
		ReRunVideoOne();
		break;
	case 2:
		ReRunVideoTwo();
		break;
	case 3:
		ReRunVideoThree();
		break;
	case 4:
		ReRunVideoFour();
		break;
	default:
		break;
	}
}

void AipuAPI::CloseWindow() {
	pipe1->SetFinishLoop();
	pipe2->SetFinishLoop();
	pipe3->SetFinishLoop();
	pipe4->SetFinishLoop();
		
}

void AipuAPI::ObserverError() {
	auto observerError1 = pipe1->observableError.map([](Either* either) {
		return either;
		});


	auto subscriptionError1 = observerError1.subscribe([this](Either* either) {
		if (either->GetLabel() != "OK")
		{
			string message = to_string(either->GetCode()) + ": " + either->GetLabel();
			messageError = message;
			cout << messageError << endl;
		}

		});

	subscriptionError1.clear();


	auto observerError2 = pipe2->observableError.map([](Either* either) {
		return either;
	});


	auto subscriptionError2 = observerError2.subscribe([this](Either* either) {
		if (either->GetLabel() != "OK")
		{
			string message = to_string(either->GetCode()) + ": " + either->GetLabel();
			messageError = message;
			cout << messageError << endl;
		}

	});

	subscriptionError2.clear();


	auto observerError3 = pipe3->observableError.map([](Either* either) {
		return either;
		});


	auto subscriptionError3 = observerError3.subscribe([this](Either* either) {
		if (either->GetLabel() != "OK")
		{
			string message = to_string(either->GetCode()) + ": " + either->GetLabel();
			messageError = message;
			cout << messageError << endl;
		}

		});

	subscriptionError3.clear();	


	auto observerError4 = pipe4->observableError.map([](Either* either) {
		return either;
		});


	auto subscriptionError4 = observerError4.subscribe([this](Either* either) {
		if (either->GetLabel() != "OK")
		{
			string message = to_string(either->GetCode()) + ": " + either->GetLabel();
			messageError = message;
			cout << messageError << endl;
		}

		});

	subscriptionError4.clear();

	
}

void AipuAPI::Terminate() {
	if (pipe1->GetIsLoadConfig())
	{
		pipe1->RemoveUnidentified();
		pipe1->CloseConnection();
	}

	if (pipe2->GetIsLoadConfig())
	{
		pipe2->RemoveUnidentified();
		pipe2->CloseConnection();
	}
	if (pipe3->GetIsLoadConfig())
	{
		pipe3->RemoveUnidentified();
		pipe3->CloseConnection();
	}
	if (pipe4->GetIsLoadConfig())
	{
		pipe4->RemoveUnidentified();
		pipe4->CloseConnection();
	}	

	listUser.clear();

	innovatrics->Terminate();

	if (isOpenWindow)
	{
		//cout << "CLOSE WINDOW AIPUAPI" << endl;
		windowOGL.DestroyWindow();
		
	}
	
	//isLoadIdentify = false;
	//flowVideo->Terminate();
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
	//flowVideo->ReloadRecognitionFace();
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


void AipuAPI::RecognitionFaceFiles(string file, int client) {
	switch (client)
	{
	case 1:
		pipe1->RecognitionFaceFiles(file, client);
		break;
	case 2:
		pipe2->RecognitionFaceFiles(file, client);
		break;
	case 3:
		pipe3->RecognitionFaceFiles(file, client);
		break;
	case 4:
		pipe4->RecognitionFaceFiles(file, client);
		break;
	default:
		break;
	}
	
}

void AipuAPI::SetIsFinishLoadFiles(int option, bool value) {
	switch (option)
	{
	case 1:
		pipe1->SetIsFinishLoadFiles(value);
		break;
	case 2:
		pipe2->SetIsFinishLoadFiles(value);
		break;
	case 3:
		pipe3->SetIsFinishLoadFiles(value);
		break;
	case 4:
		pipe4->SetIsFinishLoadFiles(value);
		break;
	default:
		break;
	}
	
}

bool AipuAPI::GetIsFinishLoadFiles(int option) {

	switch (option)
	{
	case 1:
		return pipe1->GetIsFinishLoadFiles();
		break;
	case 2:
		return pipe2->GetIsFinishLoadFiles();
		break;
	case 3:
		return pipe3->GetIsFinishLoadFiles();
		break;
	case 4:
		return pipe4->GetIsFinishLoadFiles();
		break;
	default:
		return true;
		break;
	}
	
	
}

void AipuAPI::ResetPerformance(int option) {
	switch (option)
	{
	case 1:
		pipe1->ResetPerformance();
		break;
	case 2:
		pipe2->ResetPerformance();
		break;
	case 3:
		pipe3->ResetPerformance();
		break;
	case 4:
		pipe4->ResetPerformance();
		break;
	default:
		break;
	}
}

void AipuAPI::SavePerformance(int option) {
	switch (option)
	{
	case 1:
		pipe1->SavePerformance();
		break;
	case 2:
		pipe2->SavePerformance();
		break;
	case 3:
		pipe3->SavePerformance();
		break;
	case 4:
		pipe4->SavePerformance();
		break;
	default:
		break;
	}
}


void AipuAPI::StatePlay(int option) {
	switch (option)
	{
	case 1:
		pipe1->StatePlay();
		break;
	case 2:
		pipe2->StatePlay();
		break;
	case 3:
		pipe3->StatePlay();
		break;
	case 4:
		pipe4->StatePlay();
		break;
	default:
		break;
	}
	//flowVideo->StatePlay();
}

void AipuAPI::StatePaused(int option) {
	switch (option)
	{
	case 1:
		pipe1->StatePaused();
		break;
	case 2:
		pipe2->StatePaused();
		break;
	case 3:
		pipe3->StatePaused();
		break;
	case 4:
		pipe4->StatePaused();
		break;
	default:
		break;
	}
	//flowVideo->StatePaused();
}

void AipuAPI::SetFinishLoop(int option) {
	switch (option)
	{
	case 1:
		pipe1->SetFinishLoop();
		break;
	case 2:
		pipe2->SetFinishLoop();
		break;
	case 3:
		pipe3->SetFinishLoop();
		break;
	case 4:
		pipe4->SetFinishLoop();
		break;
	default:
		break;
	}
}

void AipuAPI::SetTaskIdentify(int value, int option) {
	switch (option)
	{
	case 1:
		pipe1->SetTaskIdentify(value);
		break;
	case 2:
		pipe2->SetTaskIdentify(value);
		break;
	case 3:
		pipe3->SetTaskIdentify(value);
		break;
	case 4:
		pipe4->SetTaskIdentify(value);
		break;
	default:
		break;
	}
}

void AipuAPI::ResetEnrollVideo(int option) {
	switch (option)
	{
	case 1:
		pipe1->ResetEnrollVideo();
		break;
	case 2:
		pipe2->ResetEnrollVideo();
		break;
	case 3:
		pipe3->ResetEnrollVideo();
		break;
	case 4:
		pipe4->ResetEnrollVideo();
		break;
	default:
		break;
	}
}

void AipuAPI::AddCollectionOfImages(string folder, int client, int doing) {
	switch (client)
	{
	case 1:
		pipe1->AddCollectionOfImages(folder, client, doing);
		break;
	case 2:
		pipe2->AddCollectionOfImages(folder, client, doing);
		break;
	case 3:
		pipe3->AddCollectionOfImages(folder, client, doing);
		break;
	case 4:
		pipe4->AddCollectionOfImages(folder, client, doing);
		break;
	default:
		break;
	}
}