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
	//tracerImage += " FindUser, ";
	errorCode = faceIdkit->FindUser(data, sizeImage, &userID, &score, &tracerImage);
	error->CheckError(errorCode, error->medium);

	if (userID == 0 && errorCode == IENGINE_E_NOERROR) {

		if (lastId == 1)
		{			
			templateGuide.assign(data, data + sizeImage);
			//tracerImage += ", Add Template Guide\n";
			//file->WriteFile(tracerImage);
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
		
		templateGuide.assign(data, data + sizeImage);			
		//tracerImage += " Add Template Guide\n";
		//file->WriteFile(tracerImage);
		
	}
	else if (faceIdkit->configuration->GetIsConcatenateTemplates() == 1) 		
	{
			MatchUsers(data, sizeImage);
	}
		
	
}

void FaceIndentify::BuildTracerPrevious(string previous) {
	if (tracerPrevImage.empty())
	{
		tracerPrevImage = previous;
	}
	else
	{
		size_t pos = 0;
		string delimiter = "frame";
		pos = previous.find(delimiter);
		string withoutHead = previous.substr(pos, previous.length());
		tracerPrevImage += withoutHead;
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
		//BuildTracerPrevious(std::get<3>(modelImage));
		//tracerImage = std::get<3>(modelImage) + "EnrollUserVideo, ";
		
		tracerPrevImage = std::get<3>(modelImage);
		tracerImage = "";
		tracerMatch = "";
		//tracerFlow += "ImportUsers, ";
		tracerProcess.push_back("EnrollUserVideo");
		tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsDeduplication()));

		if (lastId == 1)
		{
			faceIdkit->ClearUser();
			BuildUserDatabase(modelImage, client, userID);
			//WriteHeadLog();
		}
		if (faceIdkit->configuration->GetIsDeduplication() == 1) {
			
			WithDeduplication(templateData, std::get<2>(modelImage)[2]);

		}
		else
		{
			tracerProcess.push_back("X");
			WithoutDeduplication(templateData, std::get<2>(modelImage)[2]);
			
		}	
		lastId++;
	}
	if (tracerImage.empty())
	{
		tracerProcess.push_back("X");
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
		tracerProcess.push_back("X");
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

		tracerProcess.push_back("Fail template");
	}
	
	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);


	templateData = NULL;
}

void FaceIndentify::AddUserEnrollVideo() {
	int errorCode, userID = -1;
	string tracerFlow = "";
	//string descriptionError = "";
	tracerProcess.push_back("EnrollUserVideo");
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsDeduplication()));

	if (!templateGuide.empty()) {
		//tracerImage = "Number of templates added: " + to_string(countAddFaceTemplate) + ", ";
		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(&templateGuide[0]);
		errorCode = faceIdkit->RegisterUser(templateData, (int)templateGuide.size(), &userID);
		error->CheckError(errorCode, error->medium);
		if (errorCode == IENGINE_E_NOERROR)
		{
			userForDatabase->SetUserIdIFace(userID);
			shootUser.on_next(userForDatabase);
		}
		/*else
		{
			const char* msgError = IEngine_GetErrorMsg(errorCode);
			string msg(msgError);
			descriptionError = ", Description: " + msg;
		}*/
		
		tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);
		templateData = NULL;
	}
	else {
		tracerFlow = "To refuse Register User";
		delete userForDatabase;
	}
	tracerProcess.push_back("X");
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsConcatenateTemplates()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenateMode()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMinimumScore()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMaximumScore()));
	tracerProcess.push_back("X");
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);

	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);

	/*string logEnd = tracerImage + tracerHeadImage ;

	file->WriteFile(logEnd);*/
	lastId = 1;
	templateGuide.clear();	
	tracerImage = "";
	tracerPrevImage = "";
	tracerMatch = " ";
	//tracerHeadImage = "";
	countAddFaceTemplate = 0;
}

void FaceIndentify::WithoutTemplatesRegisterUserWithDeduplication(std::tuple<char*, vector<unsigned char>,
	int*, string> modelImage, int client) {
	int errorCode, userID = -1, score = -1;
	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));
	string tracerFlow = "";
	tracerPrevImage = std::get<3>(modelImage);

	//tracerFlow += "ImportUsers, ";
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
		tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);
		BuildUserDatabase(modelImage, client, userID);
		shootUser.on_next(userForDatabase);
	}

	tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsConcatenateTemplates()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenateMode()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMinimumScore()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMaximumScore()));
	tracerProcess.push_back("X");
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);
	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);

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
	tracerProcess.push_back("X");
	//tracerImage = std::get<3>(modelImage);
	//tracerImage += "EnrollUserAndTemplates, Deduplication: 0, ";
	errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
	error->CheckError(errorCode, error->medium);
	tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);
	BuildUserDatabase(modelImage, client, userID);
	shootUser.on_next(userForDatabase);

	tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsConcatenateTemplates()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenateMode()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMinimumScore()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMaximumScore()));
	tracerProcess.push_back("X");
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);
	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);
	templateData = NULL;
}

void FaceIndentify::FinishRegisterUserAndTemplates() {
	int errorCode, userID = -1;
	string tracerFlow = "";
	//tracerImage = "Number of templates added: " + to_string(countAddFaceTemplate) + ", ";
	if (!templateGuide.empty())
	{
		const unsigned char* templatePtrGuide = reinterpret_cast<const unsigned char*>(&templateGuide[0]);
		errorCode = faceIdkit->RegisterUser(templatePtrGuide, (int)templateGuide.size(), &userID);
		error->CheckError(errorCode, error->medium);

		userForDatabase->SetUserIdIFace(userID);
		shootUser.on_next(userForDatabase);

		tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);
		templatePtrGuide = NULL;
	}
	else {
		tracerFlow = "To refuse Register User";
		delete userForDatabase;
	}
	if (tracerImage.empty())
	{
		tracerProcess.push_back("X");
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
		tracerProcess.push_back("X");
	}
	else {
		tracerProcess.push_back(tracerMatch);
	}
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);
	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);


	//string tracerEnd = tracerImage + tracerHeadImage;

	//file->WriteFile(tracerEnd);
	tracerImage = "";
	//tracerHeadImage = "";
	tracerPrevImage = "";
	lastId = 1;
	templateGuide.clear();	
	countAddFaceTemplate = 0;
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

		//tracerImage = "EnrollUserAndTemplates, ";
		if (lastId == 1)
		{		
			tracerPrevImage = std::get<3>(modelImage);
			tracerImage = "";
			tracerMatch = "";
			//tracerFlow += "ImportUsers, ";
			tracerProcess.push_back("EnrollUserAndTemplates");
			tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsDeduplication()));
			faceIdkit->ClearUser();
			BuildUserDatabase(modelImage, client, userID);
			//WriteHeadLog();
			
			//file->WriteFile(tracerPrevImage);
			
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

//void FaceIndentify::WriteHeadLog(string tracer) {
//	tracerPrevImage = tracer + "\n";
//	tracerHeadImage = "Similarity Threshold: " +
//		to_string(faceIdkit->GetSimilarityThreshold()) +
//		", Deduplication: " + to_string(faceIdkit->configuration->GetIsDeduplication()) +
//		", Best Candidates: " + to_string(faceIdkit->configuration->GetBestMatchedCandidates()) +
//		", Concatenate: " + to_string(faceIdkit->configuration->GetIsConcatenateTemplates()) + " ";
//	if (faceIdkit->configuration->GetIsConcatenateTemplates() == 1) {
//		tracerHeadImage += ", Concatenate mode: " + to_string(faceIdkit->configuration->GetConcatenateMode()) + " ";
//		if (faceIdkit->configuration->GetConcatenateMode() == 0)
//		{
//			tracerHeadImage += ",Minimum score: " + to_string(faceIdkit->configuration->GetConcatenationMinimumScore()) + ", ";
//			tracerHeadImage += "Maximum score: " + to_string(faceIdkit->configuration->GetConcatenationMaximumScore()) + "\n";
//
//		}
//		else
//		{
//			tracerHeadImage += "\n";
//		}
//	}
//	else
//	{
//		tracerHeadImage += "\n";
//	}
//}

void FaceIndentify::WriteHeadLog() {

	tracerHeadImage = "Similarity Threshold: " +
		to_string(faceIdkit->GetSimilarityThreshold()) +
		", Deduplication: " + to_string(faceIdkit->configuration->GetIsDeduplication()) + 
		", Best Candidates: " + to_string(faceIdkit->configuration->GetBestMatchedCandidates()) +
		", Concatenate: " + to_string(faceIdkit->configuration->GetIsConcatenateTemplates()) + " ";
	if (faceIdkit->configuration->GetIsConcatenateTemplates() == 1) {
		tracerHeadImage += ", Concatenate mode: " + to_string(faceIdkit->configuration->GetConcatenateMode()) + " ";
		if (faceIdkit->configuration->GetConcatenateMode() == 0)
		{
			tracerHeadImage += ", Minimum score: " + to_string(faceIdkit->configuration->GetConcatenationMinimumScore()) + ", ";
			tracerHeadImage += "Maximum score: " + to_string(faceIdkit->configuration->GetConcatenationMaximumScore()) + "\n";

		}
		else
		{
			tracerHeadImage += "\n";
		}
	}
	else
	{
		tracerHeadImage += "\n";
	}
	
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
				//tracerImage += "AddFaceTemplate ErrorCode: " + to_string(errorCode) + "\n";
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
			tracerMatch += "(To refuse) ";
			//file->WriteFile(tracerImage);
		}
	}
	/*else
	{
		const char* msgError = IEngine_GetErrorMsg(errorCode);
		string msg(msgError);
		descriptionError = ", Description: " + msg;
		tracerImage += "MatchUsers ErrorCode: " + to_string(errorCode) + descriptionError + "\n";
		file->WriteFile(tracerImage);
	}*/
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
	//tracerImage += ", AddFaceTemplate ErrorCode: " + to_string(errorCode) + "\n";
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
	tracerProcess.clear();
	return result;
}

void FaceIndentify::ImportUsers(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client) {
	int errorCode, userID = -1, score = -1;
	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));
	string tracerFlow = "";
	tracerPrevImage = std::get<3>(modelImage);
	
	//tracerFlow += "ImportUsers, ";
	tracerProcess.push_back("ImportUsers");
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsDeduplication()));

	if (faceIdkit->configuration->GetIsDeduplication() == 1) {			
		//tracerFlow += "Deduplication 1, FindUser, ";
		errorCode = faceIdkit->FindUser(templateData, std::get<2>(modelImage)[2], 
			&userID, &score, &tracerFlow);
		error->CheckError(errorCode, error->medium);	
		tracerProcess.push_back(tracerFlow);

		if (userID == 0 && errorCode == IENGINE_E_NOERROR) {	

			errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
			error->CheckError(errorCode, error->medium);	

			tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);
			if (errorCode == IENGINE_E_NOERROR)
			{								
				BuildUserDatabase(modelImage, client, userID);
				shootUser.on_next(userForDatabase);
			}
			//file->WriteFile(tracerFlow);
		}

	}
	else
	{
		//tracerFlow += "Deduplication 0, ";
		tracerProcess.push_back("X");
		errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
		error->CheckError(errorCode, error->medium);
		tracerFlow = "RegisterUser ErrorCode: " + to_string(errorCode);

		if (errorCode == IENGINE_E_NOERROR)
		{
			
			BuildUserDatabase(modelImage, client, userID);
			shootUser.on_next(userForDatabase);
		}
		//file->WriteFile(tracerFlow);
	}
		
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsConcatenateTemplates()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenateMode()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMinimumScore()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMaximumScore()));
	tracerProcess.push_back("X");
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);
	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);
	templateData = NULL;
}

void FaceIndentify::ControlEntry(std::tuple<char*, vector<unsigned char>, int*, string> modelImage, int client) {
	int errorCode, userID = -1, score = -1;
	userForDatabase = new User();
	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));
	string tracerFlow = "";
	tracerPrevImage = std::get<3>(modelImage);

	tracerProcess.push_back("ControlEntry");
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsDeduplication()));
	//tracerFlow += "Control Entry, FindUser, ";
	errorCode = faceIdkit->FindUser(templateData, std::get<2>(modelImage)[2], 
		&userID, &score, &tracerFlow);
	error->CheckError(errorCode, error->medium);
	tracerProcess.push_back(tracerFlow);
	//tracerFlow += "\n";
	//file->WriteFile(tracerFlow);
	if (userID == 0 && errorCode == IENGINE_E_NOERROR)
	{
		errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
		error->CheckError(errorCode, error->medium);

		userForDatabase->SetStateUser(3);		
	}
	if (errorCode == IENGINE_E_NOERROR && userID != 0) {				
		
		if (userForDatabase->GetStateUser() == 2)
		{

			userForDatabase->SetMoldScore(score);
			countRepeatUser++;
		}
		else
		{
			userForDatabase->SetMoldScore(std::get<2>(modelImage)[3]);
			countNewUser++;
		}

		if (lastUserUnidentified != userID)
		{
			if (userForDatabase->GetStateUser() == 3)
			{
				RemoveUnidentified();
				lastUserUnidentified = userID;
			}

			userForDatabase->SetUserIdIFace(userID);
			userForDatabase->SetClient(client);
			userForDatabase->SetCropImageData(std::get<1>(modelImage));
			userForDatabase->SetMoldCropHeight(std::get<2>(modelImage)[1]);
			userForDatabase->SetMoldCropWidth(std::get<2>(modelImage)[0]);
			shootUser.on_next(userForDatabase);
		}

	}
	tracerFlow = "Result ErrorCode: " + to_string(errorCode);
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetSimilarityThreshold()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetIsConcatenateTemplates()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenateMode()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMinimumScore()));
	tracerProcess.push_back(to_string(faceIdkit->configuration->GetConcatenationMaximumScore()));
	tracerProcess.push_back("X");
	tracerProcess.push_back(to_string(countAddFaceTemplate));
	tracerProcess.push_back(tracerFlow);
	tracerFlow = tracerPrevImage + BuildTracerString() + "\n";
	file->WriteFile(tracerFlow);

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

void FaceIndentify::RemoveUnidentified() {
	int errorCode;
	if (lastUserUnidentified != 0)
	{
		
		errorCode = faceIdkit->RemoveUser(lastUserUnidentified);
		error->CheckError(errorCode, error->medium);
		
	}
}

void FaceIndentify::CloseConnection() {
	faceIdkit->CloseConnection();
	templateGuide.clear();
}