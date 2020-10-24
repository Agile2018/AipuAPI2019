#include "FaceIdentify.h"

FaceIndentify::FaceIndentify()
{
	file->SetNameDirectory("Logs");
	
}

FaceIndentify::~FaceIndentify()
{	
	delete error;
	delete format;	
	delete faceIdkit;	
}

void FaceIndentify::LoadConnection() {
	faceIdkit->Connection();
	
}

void FaceIndentify::ForkTask(std::tuple<char*, 
	vector<unsigned char>, int*, string> modelImage, int client) {

	if (file->GetNameFile().length() == 0)
	{
		string nameFileTracer = file->GetNameLog() + to_string(client) + ".cvs";
		file->SetNameFile(nameFileTracer);
	}

	switch (std::get<2>(modelImage)[4])
	{
	case 0:
		ImportUsers(modelImage, client);
		break;
	case 1:
		ControlEntry(modelImage, client);
		break;
	case 2:
		EnrollUserAndTemplates(modelImage, client);
		break;
	case 3:
		EnrollUserVideo(modelImage, client);
		break;
	default:
		break;
	}

}

void FaceIndentify::WithDeduplication(const unsigned char* data, int sizeImage) {
	int errorCode, userID = -1, score = -1;
	
	errorCode = faceIdkit->FindUser(data, sizeImage, &userID, &score, &tracerImage);
	error->CheckError(errorCode, error->medium);

	if (userID == 0 && errorCode == IENGINE_E_NOERROR) {

		if (lastId == 1)
		{			
			templateGuide.clear();
			templateGuide.assign(data, data + sizeImage);
			
		}
		else {
			if (faceIdkit->configuration->GetIsConcatenateTemplates() == 1)
			{
				MatchUsers(data, sizeImage);
			}
			
		}

	}
}

void FaceIndentify::WithoutDeduplication(const unsigned char* data, int sizeImage) {
	
	if (lastId == 1)
	{
		templateGuide.clear();
		templateGuide.assign(data, data + sizeImage);			
		
		
	}
	else if (faceIdkit->configuration->GetIsConcatenateTemplates() == 1) 		
	{
			MatchUsers(data, sizeImage);
	}
		
	
}

void FaceIndentify::EnrollUserVideo(std::tuple<char*,
	vector<unsigned char>, int*, string> modelImage, int client) {
	int userID = -1;
	int tracerCountTemplates = countAddFaceTemplate;
	string tracerFlow = "";
	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));

	if (countModelSendVideo == 0)
	{		
		
		tracerPrevImage = std::get<3>(modelImage);
		tracerImage = "";
		tracerMatch = "";
		
		tracerProcess.push_back("EnrollUserVideo");
		tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsDeduplication()));

		if (lastId == 1)
		{
			faceIdkit->ClearUser();
			BuildUserDatabase(modelImage, client, userID);
			
		}
		if (faceIdkit->configuration->GetIsDeduplication() == 1) {
			
			WithDeduplication(templateData, std::get<2>(modelImage)[2]);

		}
		else
		{
			tracerProcess.push_back("0");
			WithoutDeduplication(templateData, std::get<2>(modelImage)[2]);
			
		}	
		lastId++;

		if (tracerImage.empty())
		{
			tracerProcess.push_back("0");
		}
		else {
			tracerProcess.push_back(tracerImage);
		}

		tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
		tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsConcatenateTemplates()));
		tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenateMode()));
		tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMinimumScore()));
		tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMaximumScore()));
		if (tracerMatch.empty())
		{
			tracerProcess.push_back("0");
		}
		else {
			tracerProcess.push_back(tracerMatch);
		}
		tracerProcess.push_back(to_string(countAddFaceTemplate));

		if (tracerCountTemplates < countAddFaceTemplate)
		{
			tracerProcess.push_back("Add template");
		}
		else {
			if (countAddFaceTemplate == 0 && templateGuide.size() > 0)
			{
				tracerProcess.push_back("Add guide template");
			}
			else
			{
				tracerProcess.push_back("Fail template");
			}

		}
		if (faceIdkit->configuration->GetMaximumConcatenateTemplates() != -1 && 
			faceIdkit->configuration->GetMaximumConcatenateTemplates() == countAddFaceTemplate)
		{
			countModelSendVideo = 1;
		}
		tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
		file->WriteFile(tracerFlow);
	}
	
	templateData = NULL;
}

void FaceIndentify::AddUserEnrollVideo() {
	int errorCode, userID = -1;
	string tracerFlow = "";
	
	tracerProcess.push_back("EnrollUserVideo");
	tracerProcess.push_back("-1");

	if (!templateGuide.empty()) {
		
		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(&templateGuide[0]);
		errorCode = faceIdkit->RegisterUser(templateData, (int)templateGuide.size(), &userID);
		error->CheckError(errorCode, error->medium);
		
		/*else
		{
			const char* msgError = IEngine_GetErrorMsg(errorCode);
			string msg(msgError);
			descriptionError = ", Description: " + msg;
		}*/
		
		tracerFlow = "Register Guide User ErrorCode: " + to_string(errorCode);
		templateData = NULL;
	}
	else {
		errorCode = -1;
		tracerFlow = "To refuse Register User";
		delete userForDatabase;
	}
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);

	if (errorCode == IENGINE_E_NOERROR)
	{
		string logJSON = BuildJSONLog(tracerPrevImage);
		userForDatabase->SetUserIdIFace(userID);
		userForDatabase->SetLogProcess(logJSON);
		shootUser.on_next(userForDatabase);
	}


	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);

	
	lastId = 1;
	templateGuide.clear();	
	tracerImage = "";
	tracerPrevImage = "";
	tracerMatch = " ";
	
	countAddFaceTemplate = 0;
}

void FaceIndentify::WithoutTemplatesRegisterUserWithDeduplication(std::tuple<char*, vector<unsigned char>,
	int*, string> modelImage, int client) {
	int errorCode, userID = -1, score = -1;
	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));
	string tracerFlow = "";
	tracerPrevImage = std::get<3>(modelImage);

	
	tracerProcess.push_back("EnrollUserAndTemplates");
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsDeduplication()));

	//tracerImage = std::get<3>(modelImage);
	//tracerImage += "EnrollUserAndTemplates, Deduplication: 1, FindUser, ";

	errorCode = faceIdkit->FindUser(templateData, std::get<2>(modelImage)[2],
		&userID, &score, &tracerFlow);
	error->CheckError(errorCode, error->medium);
	tracerProcess.push_back(tracerFlow);
	if (userID == 0 && errorCode == IENGINE_E_NOERROR) {
		errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
		error->CheckError(errorCode, error->medium);				
	}
	tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsConcatenateTemplates()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenateMode()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMinimumScore()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMaximumScore()));
	tracerProcess.push_back("-1");
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);

	string logJSON = BuildJSONLog(tracerPrevImage);
	BuildUserDatabase(modelImage, client, userID);
	shootUser.on_next(userForDatabase);

	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);
	tracerProcess.clear();
	tracerPrevImage = "";
	templateData = NULL;
}

void FaceIndentify::WithoutTemplatesRegisterUserWithoutDeduplication(std::tuple<char*, vector<unsigned char>,
	int*, string> modelImage, int client) {
	int errorCode, userID = -1;

	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));

	string tracerFlow = "";
	tracerPrevImage = std::get<3>(modelImage);	
	tracerProcess.push_back("EnrollUserAndTemplates");
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsDeduplication()));
	tracerProcess.push_back("-1");
	
	errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
	error->CheckError(errorCode, error->medium);
	tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);
	
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsConcatenateTemplates()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenateMode()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMinimumScore()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMaximumScore()));
	tracerProcess.push_back("-1");
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);

	string logJSON = BuildJSONLog(tracerPrevImage);
	BuildUserDatabase(modelImage, client, userID);
	shootUser.on_next(userForDatabase);

	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);
	tracerProcess.clear();
	tracerPrevImage = "";
	templateData = NULL;
}

void FaceIndentify::FinishRegisterUserAndTemplates() {
	int errorCode, userID = -1;
	string tracerFlow = "";
	
	if (!templateGuide.empty())
	{
		const unsigned char* templatePtrGuide = reinterpret_cast<const unsigned char*>(&templateGuide[0]);
		errorCode = faceIdkit->RegisterUser(templatePtrGuide, (int)templateGuide.size(), &userID);
		error->CheckError(errorCode, error->medium);

		userForDatabase->SetUserIdIFace(userID);
		
		tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);
		templatePtrGuide = NULL;
	}
	else {
		errorCode = -1;
		tracerFlow = "To refuse Register User";
		delete userForDatabase;
	}
	if (tracerImage.empty())
	{
		tracerProcess.push_back("0");
	}
	else {
		tracerProcess.push_back(tracerImage);
	}
	
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsConcatenateTemplates()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenateMode()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMinimumScore()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMaximumScore()));
	if (tracerMatch.empty())
	{
		tracerProcess.push_back("0");
	}
	else {
		tracerProcess.push_back(tracerMatch);
	}
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);
	if (errorCode == IENGINE_E_NOERROR) {
		string logJSON = BuildJSONLog(tracerPrevImage);
		userForDatabase->SetLogProcess(logJSON);
		shootUser.on_next(userForDatabase);
	}


	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);

	tracerImage = "";
	
	
	lastId = 1;
	templateGuide.clear();	
	countAddFaceTemplate = 0;
	//clock_t timeStart1 = clock();
	//string logJSON = BuildJSONLog(tracerPrevImage);
	//cout << "JSON STRING: " << logJSON << endl;
	/*clock_t duration1 = clock() - timeStart1;
	int durationMs1 = int(1000 * ((float)duration1) / CLOCKS_PER_SEC);
	printf("   BUILD JASON LOG time: %d\n", durationMs1);*/
	tracerProcess.clear();
	tracerPrevImage = "";
}

void FaceIndentify::EnrollUserAndTemplates(std::tuple<char*,
	vector<unsigned char>, int*, string> modelImage, int client) {
	int userID = -1;

	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));

	if (std::get<2>(modelImage)[5] == 1) {

		if (faceIdkit->configuration->GetIsDeduplication() == 1) {
			WithoutTemplatesRegisterUserWithDeduplication(modelImage, client);
			
		}
		else {
			WithoutTemplatesRegisterUserWithoutDeduplication(modelImage, client);
			
		}
		lastId = 1;
	}
	else {

		
		if (lastId == 1)
		{		
			tracerPrevImage = std::get<3>(modelImage);
			tracerImage = "";
			tracerMatch = "";
			
			tracerProcess.push_back("EnrollUserAndTemplates");
			tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsDeduplication()));
			faceIdkit->ClearUser();
			BuildUserDatabase(modelImage, client, userID);
			
			
		}

		if (faceIdkit->configuration->GetIsDeduplication() == 1) {

			WithDeduplication(templateData, std::get<2>(modelImage)[2]);

		}
		else {

			WithoutDeduplication(templateData, std::get<2>(modelImage)[2]);

		}
		if (std::get<2>(modelImage)[5] == lastId ) {
			FinishRegisterUserAndTemplates();			
		}
		else {
			lastId++;
		}		
	}
	
	templateData = NULL;
}


void FaceIndentify::BuildUserDatabase(std::tuple<char*, 
	vector<unsigned char>, int*, string> modelImage, int client, int userId) {
	userForDatabase = new User();
	userForDatabase->SetStateUser(1);
	userForDatabase->SetMoldScore(std::get<2>(modelImage)[3]);
	userForDatabase->SetUserIdIFace(userId);
	userForDatabase->SetClient(client);
	userForDatabase->SetCropImageData(std::get<1>(modelImage));
	userForDatabase->SetMoldCropHeight(std::get<2>(modelImage)[1]);
	userForDatabase->SetMoldCropWidth(std::get<2>(modelImage)[0]);
	
}


void FaceIndentify::GetFaceTemplateUser(int idUser, int index) {
	int lenght;
	unsigned char* templatePattern = faceIdkit->GetFaceTemplate(idUser, &lenght, index);
	templateGuide.clear();
	templateGuide.assign(templatePattern, templatePattern + lenght);	
	templatePattern = NULL;
}

void FaceIndentify::ConcatenateModeAuto(const unsigned char* templateIn, int sizeIn) {
	int errorCode, score = -1;
	string descriptionError = "";

	vector<unsigned char> templateNew(templateIn, templateIn + sizeIn);
	const unsigned char* templatePtrNew = reinterpret_cast<const unsigned char*>(&templateNew[0]);
	const unsigned char* templatePtrGuide = reinterpret_cast<const unsigned char*>(&templateGuide[0]);
	errorCode = faceIdkit->MatchUsers(templatePtrGuide,
		(int)templateGuide.size(), templatePtrNew, sizeIn, &score);
	error->CheckError(errorCode, error->medium);
	tracerMatch += to_string(score) + " ";
	if (errorCode == IENGINE_E_NOERROR) {
		if (score <= faceIdkit->configuration->GetConcatenationMaximumScore() &&
			score >= faceIdkit->configuration->GetConcatenationMinimumScore())
		{
			errorCode = faceIdkit->AddFaceTemplate(templatePtrNew, sizeIn);
			error->CheckError(errorCode, error->medium);
			if (errorCode == IENGINE_E_NOERROR)
			{
				countAddFaceTemplate++;
				
			}
			/*else {
				const char* msgError = IEngine_GetErrorMsg(errorCode);
				string msg(msgError);
				descriptionError = ", Description: " + msg;
				tracerImage += "AddFaceTemplate ErrorCode: " + to_string(errorCode) + descriptionError + "\n";

			}
			file->WriteFile(tracerImage);*/

		}
		else
		{
			tracerMatch += "(To_refuse) ";
			
		}
	}
	
	templateNew.clear();
	templatePtrNew = NULL;
	templatePtrGuide = NULL;
}

void FaceIndentify::ConcatenateModeForce(const unsigned char* templateIn, int sizeIn) {
	int errorCode;

	errorCode = faceIdkit->AddFaceTemplate(templateIn, sizeIn);
	error->CheckError(errorCode, error->medium);
	if (errorCode == IENGINE_E_NOERROR)
	{
		countAddFaceTemplate++;
	}
	
}

void FaceIndentify::MatchUsers(const unsigned char* templateIn, int sizeIn) {
		
	if (faceIdkit->configuration->GetConcatenateMode() == 0) {		
		ConcatenateModeAuto(templateIn, sizeIn);		
	}
	else
	{
		ConcatenateModeForce(templateIn, sizeIn);
				
	}
	
}

string FaceIndentify::BuildTracerString() {
	string result = "";
	for (string value : tracerProcess) {
		result += value + ", ";
	}
	//tracerProcess.clear();
	return result;
}

void FaceIndentify::ImportUsers(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client) {
	int errorCode, userID = -1, score = -1;
	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));
	string tracerFlow = "";
	tracerPrevImage = std::get<3>(modelImage);
	
	
	tracerProcess.push_back("ImportUsers");
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsDeduplication()));

	if (faceIdkit->configuration->GetIsDeduplication() == 1) {			
		
		errorCode = faceIdkit->FindUser(templateData, std::get<2>(modelImage)[2], 
			&userID, &score, &tracerFlow);
		error->CheckError(errorCode, error->medium);	
		tracerProcess.push_back(tracerFlow);
		tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
		if (userID == 0 && errorCode == IENGINE_E_NOERROR) {	

			errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
			error->CheckError(errorCode, error->medium);	

			/*tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);
			if (errorCode == IENGINE_E_NOERROR)
			{								
				BuildUserDatabase(modelImage, client, userID);
				shootUser.on_next(userForDatabase);
			}*/
			
		}

	}
	else
	{
		
		tracerProcess.push_back("-1");
		tracerProcess.push_back("-1");
		errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
		error->CheckError(errorCode, error->medium);
		/*tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);

		if (errorCode == IENGINE_E_NOERROR)
		{
			
			BuildUserDatabase(modelImage, client, userID);
			shootUser.on_next(userForDatabase);
		}*/
		
	}
		
	tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");

	tracerProcess.push_back("-1");
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);
	

	if (errorCode == IENGINE_E_NOERROR)
	{
		string logJSON = BuildJSONLog(tracerPrevImage);		
		BuildUserDatabase(modelImage, client, userID);
		userForDatabase->SetLogProcess(logJSON);
		shootUser.on_next(userForDatabase);
	}

	//tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
	/*tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsConcatenateTemplates()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenateMode()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMinimumScore()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMaximumScore()));*/
	


	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);
	tracerProcess.clear();
	tracerPrevImage = "";
	templateData = NULL;
}

void FaceIndentify::ControlEntry(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client) {
	int errorCode, userID = -1, score = -1;
	userForDatabase = new User();
	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));
	string tracerFlow = "";
	tracerPrevImage = std::get<3>(modelImage);

	tracerProcess.push_back("ControlEntry");
	tracerProcess.push_back("-1"); // deduplication
	//tracerFlow += "Control Entry, FindUser, ";
	errorCode = faceIdkit->FindUser(templateData, std::get<2>(modelImage)[2], 
		&userID, &score, &tracerFlow);
	error->CheckError(errorCode, error->medium);
	tracerProcess.push_back(tracerFlow);
	tracerFlow = "";
	if (userID == 0 && errorCode == IENGINE_E_NOERROR)
	{
		
		if (CheckUserUnidentified(templateData, std::get<2>(modelImage)[2]))
		{
			userForDatabase->SetStateUser(3);
			userID = distribution(generator) * -1;						
		}
		tracerFlow = "User does not exist.";
		
	}
	

	if (tracerFlow.empty())
	{
		tracerFlow = "Result ErrorCode: " + to_string(errorCode);
	}

	tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back("-1");
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);
	

	if (userForDatabase->GetStateUser() == 3 || 
		userID != 0 &&
		errorCode == IENGINE_E_NOERROR)
	{
		string logJSON = BuildJSONLog(tracerPrevImage);
		userForDatabase->SetUserIdIFace(userID);
		userForDatabase->SetClient(client);
		userForDatabase->SetMoldScore(score);
		userForDatabase->SetCropImageData(std::get<1>(modelImage));
		userForDatabase->SetMoldCropHeight(std::get<2>(modelImage)[1]);
		userForDatabase->SetMoldCropWidth(std::get<2>(modelImage)[0]);
		userForDatabase->SetLogProcess(logJSON);
		shootUser.on_next(userForDatabase);
	}
	
	
	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);		
	tracerProcess.clear();
	tracerPrevImage = "";
	templateData = NULL;

}

void FaceIndentify::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
		shootError.on_next(either);
	});

	auto faceIdkitError = faceIdkit->observableError.map([](Either* either) {
		return either;
		});

	auto subscriptionFaceIdkitError = faceIdkitError.subscribe([this](Either* either) {
		shootError.on_next(either);
		});
}

void FaceIndentify::CloseConnection() {
	faceIdkit->CloseConnection();
	templateGuide.clear();
}

bool FaceIndentify::CheckUserUnidentified(const unsigned char* data, int size) {
	int errorCode, score = -1;
	if (!templateGuide.empty())
	{
		const unsigned char* templatePtrGuide = reinterpret_cast<const unsigned char*>(&templateGuide[0]);
		errorCode = faceIdkit->MatchUsers(templatePtrGuide,
			(int)templateGuide.size(), data, size, &score);
		error->CheckError(errorCode, error->medium);
		
		if (errorCode == IENGINE_E_NOERROR && score == 0)
		{
			templateGuide.clear();
			templateGuide.assign(data, data + size);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		templateGuide.assign(data, data + size);
	}
	return true;
	
}

std::vector<string> FaceIndentify::TransformLogToVector(string content, string delimiter) {
	std::vector<string> tracerPrevious;
	if (!content.empty())
	{
		
		size_t pos = 0;
		size_t prev = 0;
		bool flag = true;

		while (flag)
		{
			pos = content.find(delimiter);
			if (pos != std::string::npos) {				
				string remnant = content.substr(prev, pos);
				if (!remnant.empty())
				{
					tracerPrevious.push_back(remnant);
				}
				
				content = content.substr(pos + 1, content.length());
				//cout << "             CONTENT: " << content << endl;
				
			}
			else
			{
				
				flag = false;
			}
		}				

	}
	
	return tracerPrevious;
}

string FaceIndentify::BuildJSONLog(string prevContent) {	
	std::vector<string> tracerPrevious = TransformLogToVector(prevContent, ",");
	
	if (!tracerPrevious.empty())
	{		
		//cout << "Size Tracer Prev: " << tracerPrevious.size() << endl;
		turnIntoLogToJSON->jsonBody.clear();
		turnIntoLogToJSON->jsonParams.clear();
		turnIntoLogToJSON->jsonArray.clear();

		std::map<std::string, std::string> params;

		params.insert(std::pair<std::string, std::string>("Date", tracerPrevious[0]));
		params.insert(std::pair<std::string, std::string>("Channel", tracerPrevious[1]));
		params.insert(std::pair<std::string, std::string>("Source", tracerPrevious[2]));
		params.insert(std::pair<std::string, std::string>("Quantity_Flow", tracerPrevious[3]));

		std::map<std::string, std::string>::const_iterator it = params.begin(),
			end = params.end();

		for (; it != end; ++it) {
			turnIntoLogToJSON->jsonBody[it->first] = it->second;

		}

		std::vector<string> tracerSize = TransformLogToVector(tracerPrevious[4], " ");
		if (tracerSize.empty())
		{
			turnIntoLogToJSON->jsonBody["Size_Image"] = tracerPrevious[4];
		}
		else {

			for (size_t i = 0; i < tracerSize.size(); i++)
			{
				turnIntoLogToJSON->jsonArray[(int)i][to_string((int)i + 1)] = tracerSize[(int)i];
			}

			turnIntoLogToJSON->jsonBody["Size_Image"] = turnIntoLogToJSON->jsonArray;
		}
		std::vector<string> tracerConfidence = TransformLogToVector(tracerPrevious[5], " ");

		turnIntoLogToJSON->jsonArray.clear();

		if (tracerConfidence.empty())
		{
			turnIntoLogToJSON->jsonBody["Confidence_Threshold"] = tracerPrevious[5];
		}
		else {

			for (size_t i = 0; i < tracerConfidence.size(); i++)
			{
				turnIntoLogToJSON->jsonArray[(int)i][to_string((int)i + 1)] = tracerConfidence[(int)i];
			}

			turnIntoLogToJSON->jsonBody["Confidence_Threshold"] = turnIntoLogToJSON->jsonArray;
		}

		turnIntoLogToJSON->jsonArray.clear();
		turnIntoLogToJSON->jsonBody["Param_Confidence_Threshold"] = tracerPrevious[6];
		turnIntoLogToJSON->jsonBody["Quantity_Up_Confidence"] = tracerPrevious[7];
		turnIntoLogToJSON->jsonArray.clear();
		std::vector<string> tracerModels = TransformLogToVector(tracerPrevious[8], " ");

		if (tracerModels.empty())
		{
			turnIntoLogToJSON->jsonBody["Template_Quality"] = tracerPrevious[8];
		}
		else {

			for (size_t i = 0; i < tracerModels.size(); i++)
			{
				turnIntoLogToJSON->jsonArray[(int)i][to_string((int)i + 1)] = tracerModels[(int)i];
			}

			turnIntoLogToJSON->jsonBody["Template_Quality"] = turnIntoLogToJSON->jsonArray;
		}

		turnIntoLogToJSON->jsonArray.clear();
		turnIntoLogToJSON->jsonBody["Param_Template_Quality"] = tracerPrevious[9];
		turnIntoLogToJSON->jsonBody["Quantity_Up_Template"] = tracerPrevious[10];
		
		turnIntoLogToJSON->jsonBody["Function"] = tracerProcess[0];
		turnIntoLogToJSON->jsonBody["Param_Deduplication"] = tracerProcess[1];
		std::vector<string> tracerFind = TransformLogToVector(tracerProcess[2], " ");

		if (tracerFind.empty())
		{
			turnIntoLogToJSON->jsonBody["FindUser"] = tracerPrevious[2];
		}
		else {

			for (size_t i = 0; i < tracerFind.size(); i++)
			{
				turnIntoLogToJSON->jsonArray[(int)i][to_string((int)i + 1)] = tracerFind[(int)i];
			}

			turnIntoLogToJSON->jsonBody["FindUser"] = turnIntoLogToJSON->jsonArray;
		}
		turnIntoLogToJSON->jsonArray.clear();
		turnIntoLogToJSON->jsonBody["Param_Similarity_Threshold"] = tracerProcess[3];
		turnIntoLogToJSON->jsonBody["Param_Concatenate"] = tracerProcess[4];
		turnIntoLogToJSON->jsonBody["Param_Concatenate_Mode"] = tracerProcess[5];
		turnIntoLogToJSON->jsonBody["Param_Minimum_Score"] = tracerProcess[6];
		turnIntoLogToJSON->jsonBody["Param_Maximum_Score"] = tracerProcess[7];
		std::vector<string> tracerMatch = TransformLogToVector(tracerProcess[8], " ");

		if (tracerMatch.empty())
		{
			turnIntoLogToJSON->jsonBody["Match_Score"] = tracerProcess[8];
		}
		else {

			for (size_t i = 0; i < tracerMatch.size(); i++)
			{
				turnIntoLogToJSON->jsonArray[(int)i][to_string((int)i + 1)] = tracerMatch[(int)i];
			}

			turnIntoLogToJSON->jsonBody["Match_Score"] = turnIntoLogToJSON->jsonArray;
		}

		turnIntoLogToJSON->jsonArray.clear();
		turnIntoLogToJSON->jsonBody["Templates"] = tracerProcess[9];
		turnIntoLogToJSON->jsonBody["Result"] = tracerProcess[10];
			

		turnIntoLogToJSON->WriteJSONToString();

		return turnIntoLogToJSON->stringJSON;

	}

	return "JASON FAIL...........";
}