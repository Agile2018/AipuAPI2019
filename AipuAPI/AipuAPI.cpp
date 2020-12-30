#include "AipuAPI.h"
#include "Innovatrics.h"
#include "Pipe.h"

Innovatrics* innovatrics;
Database databaseMongo;
File* fileErrors;
FrameView windowOGL;
Format* format = new Format();
vector<Pipe*> pipelines;
std::vector<std::thread> threadPipes;
std::mutex mtx;
string userJson;
string messageError;
bool isOpenWindow = false;
std::vector<string> listUser;
std::vector<std::string> listTemplates;
int countTemplates = 0;
const string nameConfiguration =  "configuration";
const string fileDatabaseConfiguration = "database.txt";
const string fileGlobalConfiguration = "global.txt";
const string folderLogs = "Logs";
const string nameFileErrors = "errors.txt";

const char* fileConfigurations[4] = { "configPipeOne.txt", "configPipeTwo.txt", 
"configPipeThree.txt", "configPipeFour.txt" };

std::vector<int> GetChannels(string channels)
{
	std::vector<int> result;
	std::vector<string> strChannels = format->Split(channels, '*');
	for (int i = 0; i < strChannels.size(); i++)
	{
		int newValue = stoi(strChannels[i]) - 1;
		result.push_back(newValue);
	}
	return result;
}

void AipuAPI::SetNumberPipelines(int value) {
	for (int i = 0; i < pipelines.size(); i++)
	{
		pipelines[i]->DownConfigurationModel();
		pipelines[i]->CloseConnectionIdentification();
	}
	pipelines.clear();
	for (int i = 0; i < value; i++)
	{
		Pipe* pipe = new Pipe();
		pipelines.push_back(pipe);
	}
	ObserverError();
	ObserverTemplateJson();
	pipelines[0]->SetDatabase(&databaseMongo);
}

void ThreadRunWindow(int idx) {		
	windowOGL.SetIndexCurrent(idx);
	int end = windowOGL.RunOne();
	pipelines[idx]->SetFinishLoop();	
	pipelines[idx]->DownConfigurationModel();
	pipelines[idx]->CloseConnectionIdentification();
	pipelines[idx]->ResetConfiguration();
	threadPipes.clear();
	messageError = "FINISH LOOP";
	cout << "FINISH LOOP" << endl;
}

void ThreadRunWindowTwo(int idx0, int idx1) {		
	int end = windowOGL.RunTwo();
	pipelines[idx0]->SetFinishLoop();
	pipelines[idx0]->DownConfigurationModel();
	pipelines[idx0]->CloseConnectionIdentification();
	pipelines[idx0]->ResetConfiguration();
	pipelines[idx1]->SetFinishLoop();
	pipelines[idx1]->DownConfigurationModel();
	pipelines[idx1]->CloseConnectionIdentification();
	pipelines[idx1]->ResetConfiguration();
	threadPipes.clear();
	messageError = "FINISH LOOP";
	cout << "FINISH LOOP" << endl;

}

void ThreadRunWindowThree(int idx0, int idx1, int idx2) {
	int end = windowOGL.RunThree();
	pipelines[idx0]->SetFinishLoop();
	pipelines[idx0]->DownConfigurationModel();
	pipelines[idx0]->CloseConnectionIdentification();
	pipelines[idx0]->ResetConfiguration();
	pipelines[idx1]->SetFinishLoop();
	pipelines[idx1]->DownConfigurationModel();
	pipelines[idx1]->CloseConnectionIdentification();
	pipelines[idx1]->ResetConfiguration();
	pipelines[idx2]->SetFinishLoop();
	pipelines[idx2]->DownConfigurationModel();
	pipelines[idx2]->CloseConnectionIdentification();
	pipelines[idx2]->ResetConfiguration();
	threadPipes.clear();
	messageError = "FINISH LOOP";
	cout << "FINISH LOOP" << endl;

}

void ThreadRunWindowFour(int idx0, int idx1, int idx2, int idx3) {
	int end = windowOGL.RunFour();
	pipelines[idx0]->SetFinishLoop();
	pipelines[idx0]->DownConfigurationModel();
	pipelines[idx0]->CloseConnectionIdentification();
	pipelines[idx0]->ResetConfiguration();
	pipelines[idx1]->SetFinishLoop();
	pipelines[idx1]->DownConfigurationModel();
	pipelines[idx1]->CloseConnectionIdentification();
	pipelines[idx1]->ResetConfiguration();
	pipelines[idx2]->SetFinishLoop();
	pipelines[idx2]->DownConfigurationModel();
	pipelines[idx2]->CloseConnectionIdentification();
	pipelines[idx2]->ResetConfiguration();
	pipelines[idx3]->SetFinishLoop();
	pipelines[idx3]->DownConfigurationModel();
	pipelines[idx3]->CloseConnectionIdentification();
	pipelines[idx3]->ResetConfiguration();
	threadPipes.clear();
	messageError = "FINISH LOOP";
	cout << "FINISH LOOP" << endl;

}


void InitWindow(int idx) {
	std::thread togl(ThreadRunWindow, idx);
	togl.detach();
	
	pipelines[0]->SetFrameView(&windowOGL);
	//pipelines[idx]->SetIndexFrame(1);
}

void InitWindowTwo(int idx0, int idx1) {
	std::thread togl(ThreadRunWindowTwo, idx0, idx1);
	togl.detach();
	
	pipelines[0]->SetFrameView(&windowOGL);
}

void InitWindowThree(int idx0, int idx1, int idx2) {
	std::thread togl(ThreadRunWindowThree, idx0, idx1, idx2);
	togl.detach();

	pipelines[0]->SetFrameView(&windowOGL);
}

void InitWindowFour(int idx0, int idx1, int idx2, int idx3) {
	std::thread togl(ThreadRunWindowFour, idx0, idx1, idx2, idx3);
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

bool AipuAPI::GetIsLoadConfiguration(int channel) {
	int index = channel - 1;
	return pipelines[index]->GetIsLoadConfig();
}

void AipuAPI::CloseConnectionIdentification(int channel) {
	int index = channel - 1;

	pipelines[index]->CloseConnectionIdentification();
}

void AipuAPI::LoadConnectionIdentification(int channel) {
	int index = channel - 1;

	pipelines[index]->LoadConnectionIdentification();
}

void AipuAPI::DownConfigurationModel(int channel) {
	int index = channel - 1;

	pipelines[index]->DownConfigurationModel();
}

void AipuAPI::LoadConfigurationModel(int channel) {
	int index = channel - 1;

	pipelines[index]->LoadConfigurationModel();
}

void AipuAPI::LoadConfigurationIdentify(int channel) {
	int index = channel - 1;

	pipelines[index]->LoadConfigurationIdentify();
}

void AipuAPI::LoadConfigurationTracking(int channel) {
	int index = channel - 1;

	pipelines[index]->LoadConfigurationTracking();
}

void AipuAPI::LoadConfigurationPipe(int pipeline) {
	int index = pipeline - 1;

	if (!pipelines[index]->GetIsLoadConfig())
	{
		LoadOnlyOnePipe(index);
	}

}

int AipuAPI::GetTaskIdentify(int channel) {
	int index = channel - 1;

	return pipelines[index]->GetTaskIdentify();
}

void AipuAPI::InitWindowMain(int option, string channels) {
	std::vector<int> vecChannels = GetChannels(channels);
	
	switch (option)
	{
	case 1:
		
		InitWindow(vecChannels[0]);
		break;
	case 2:
		
		InitWindowTwo(vecChannels[0], vecChannels[1]);
		break;
	case 3:
		
		InitWindowThree(vecChannels[0], vecChannels[1], vecChannels[2]);
		break;
	case 4:
		
		InitWindowFour(vecChannels[0], vecChannels[1], vecChannels[2], vecChannels[3]);
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
	fileErrors = new File();
	fileErrors->SetNameDirectory(folderLogs);
	fileErrors->SetNameFile(nameFileErrors);
}

AipuAPI::~AipuAPI()
{
	delete innovatrics;
	pipelines.clear();
	threadPipes.clear();
}

void AipuAPI::WriteError(string msgError) {
	char buff[20];

	time_t now = time(NULL);
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
	string timeEvent(buff);

	string messageToWrite = timeEvent + " " + msgError;
	fileErrors->WriteFile(messageToWrite);
}

void AipuAPI::ConnectDatabase() {
	InitDatabaseMongo();
	ObserverDatabase();

}

void AipuAPI::ObserverTemplateJson() {
	vector<rxcpp::observable<string>> observersTemplates;
	for (int i = 0; i < pipelines.size(); i++)
	{
		observersTemplates.push_back(pipelines[i]->observableCropImage.map([](string jsonTemplate) {
			return jsonTemplate;
		}));
	}


	vector<rxcpp::subscription> subscriptionsTemplates;
	for (int i = 0; i < pipelines.size(); i++)
	{
		subscriptionsTemplates.push_back(observersTemplates[i].subscribe([this](string jsonTemplate) {
			
			//templateJson = jsonTemplate;
			/*mtx.lock();
			listTemplates.push_back(jsonTemplate);			
			mtx.unlock();*/
			std::lock_guard<std::mutex> guard(mtx);			
			listTemplates.push_back(jsonTemplate);
			
		}));

	}

	

}

void AipuAPI::ObserverDatabase() {
	auto databaseError = databaseMongo.observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionDatabaseError = databaseError.subscribe([this](Either* either) {
		if (either->GetLabel() != "OK")
		{
			string message = to_string(either->GetCode()) + ": " + either->GetLabel();
			WriteError(message);
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

		
	});

	subscriptionDatabase.clear();

}

void ExecuteFlowVideo(int indexthread, int indexPipe) {

	pipelines[indexPipe]->RunFlowVideo();
	pipelines[indexPipe]->SetIndexthread(indexthread);
}

void AipuAPI::RunVideo(int option, string channels) {
	std::vector<int> vecChannels = GetChannels(channels);
	
	for (int i = 0; i < option; i++)
	{
		threadPipes.push_back(std::thread(ExecuteFlowVideo, i, vecChannels[i]));
		threadPipes[i].detach();
	}

	
}

void AipuAPI::ReRunVideo(int option) {
	int index = option - 1;
	int indexThread = pipelines[index]->GetIndexthread();
	auto it = threadPipes.begin();
	threadPipes.insert(it + indexThread, std::thread(ExecuteFlowVideo, indexThread, index));
	threadPipes[indexThread].detach();

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
				WriteError(message);
				messageError = message;		
				
			}
		}));
		
	}
	
}

void AipuAPI::Terminate() {

	for (int i = 0; i < pipelines.size(); i++)
	{
		/*if (pipelines[i]->GetIsLoadConfig())
		{
			
			pipelines[i]->CloseConnection();
		}*/
		pipelines[i]->DownConfigurationModel();
		pipelines[i]->CloseConnectionIdentification();
		pipelines[i]->SetFinishLoop();
	}	

	listUser.clear();
	listTemplates.clear();
	innovatrics->Terminate();

	if (isOpenWindow)
	{
		
		windowOGL.DestroyWindow();
		
	}
	
	
}

void AipuAPI::SetColourLabelFrame(int indexFrame, float red, float green, float blue) {
	windowOGL.SetColourLabelFrame(indexFrame, red, green, blue);
}

//void AipuAPI::SetColourTextFrameOne(float red, float green, float blue) {
//	windowOGL.SetColourTextFrameOne(red, green, blue);
//}
//
//void AipuAPI::SetColourTextFrameTwo(float red, float green, float blue) {
//	windowOGL.SetColourTextFrameTwo(red, green, blue);
//}
//
//void AipuAPI::SetColourTextFrameThree(float red, float green, float blue) {
//	windowOGL.SetColourTextFrameThree(red, green, blue);
//}
//
//void AipuAPI::SetColourTextFrameFour(float red, float green, float blue) {
//	windowOGL.SetColourTextFrameFour(red, green, blue);
//}

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
		listUser.erase(listUser.begin());
	}

	return user;
}

string AipuAPI::GetTemplateJSON() {
	string templateImage = "";
	
	//if (mtx.try_lock())
	//{
	//	try
	//	{
	//		if (listTemplates.size() != 0)
	//		{
	//			//templateImage = *listTemplates.begin();
	//			//cout << templateImage << " ...........LENGHT: " << templateImage.length() << endl;
	//			//listTemplates.erase(listTemplates.begin());
	//			templateImage = listTemplates[countTemplates];
	//			countTemplates++;
	//		}
	//	}
	//	catch (const std::exception& e)
	//	{
	//		cout << "ERROR GET TEMPLATE JSON: " << e.what() << endl;
	//		
	//	}
	//	
	//	
	//	mtx.unlock();
	//}
	
	if (!listTemplates.empty())
	{		
		
		templateImage = listTemplates[countTemplates];
		countTemplates++;
	}
	
	return templateImage;
}

string AipuAPI::GetMessageError() {
	string msg = messageError;
	messageError = "";
	return msg;
}


void AipuAPI::RecognitionFaceFiles(string namefile, int client, int task) {
	int index = client - 1;
	pipelines[index]->RecognitionFaceFiles(namefile, client, task);
	
}


void AipuAPI::SetIsFinishLoadFiles(int option, bool value) {

	int index = option - 1;
	pipelines[index]->SetIsFinishLoadFiles(value);
	
}

bool AipuAPI::GetIsFinishLoadFiles(int option) {
	int index = option - 1;
	return pipelines[index]->GetIsFinishLoadFiles();
	
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

void AipuAPI::ResetEnrollVideo(int option, int value) {
	int index = option - 1;
	if (value == 0)
	{
		listTemplates.clear();		
		countTemplates = value;
	}
		
	pipelines[index]->ResetEnrollVideo(value);
	
}

void AipuAPI::AddCollectionOfImages(string folder, int client, int doing) {
	int index = client - 1;
	listTemplates.clear();		
	countTemplates = 0;
	pipelines[index]->AddCollectionOfImages(folder, client, doing);

}

void AipuAPI::AddUserEnrollVideo(int channel) {
	int index = channel - 1;
	pipelines[index]->AddUserEnrollVideo();
}