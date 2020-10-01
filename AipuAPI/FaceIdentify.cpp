#include "FaceIdentify.h"

FaceIndentify::FaceIndentify()
{
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
	vector<unsigned char>, int*> modelImage, int client) {

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


void FaceIndentify::EnrollUserVideo(std::tuple<char*,
	vector<unsigned char>, int*> modelImage, int client) {
	int errorCode, userID = -1, score = -1;
	
	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));

	if (countModelSendVideo == 0)
	{
		lastId = 0;

		if (faceIdkit->configuration->GetIsDeduplication() == 1) {
			int paramSimilarity = faceIdkit->configuration->GetSimilarityThresholdDeduplication();
			faceIdkit->configuration->SetSimilarityThreshold(paramSimilarity);

			errorCode = faceIdkit->FindUser(templateData, std::get<2>(modelImage)[2], &userID, &score);
			error->CheckError(errorCode, error->medium);
			if (userID == 0 && errorCode == IENGINE_E_NOERROR) {
				errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
				error->CheckError(errorCode, error->medium);
				if (errorCode == IENGINE_E_NOERROR)
				{
					templateForMatch = NULL;
					lenghtMatch = 0;
					lastId = userID;
					countModelSendVideo++;					
					BuildUserDatabase(modelImage, client, lastId);

				}
			}
		}
		else
		{
			errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
			error->CheckError(errorCode, error->medium);
			if (errorCode == IENGINE_E_NOERROR)
			{
				templateForMatch = NULL;
				lenghtMatch = 0;
				lastId = userID;
				countModelSendVideo++;				
				BuildUserDatabase(modelImage, client, lastId);

			}
		}		
	}
	else {
		if (lastId != 0 && faceIdkit->configuration->GetIsConcatenateTemplates() == 1)
		{
			
			if (templateForMatch == NULL)
			{
				GetFaceTemplateUser();

			}
			MatchUsers(templateData, std::get<2>(modelImage)[2]);
		}

	}
	templateData = NULL;
}

void FaceIndentify::EnrollUserAndTemplates(std::tuple<char*,
	vector<unsigned char>, int*> modelImage, int client) {
	int errorCode, userID = -1, score = -1;


	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));

	if (std::get<2>(modelImage)[5] == 0)
	{
		lastId = 0;

		if (faceIdkit->configuration->GetIsDeduplication() == 1) {

			int paramSimilarity = faceIdkit->configuration->GetSimilarityThresholdDeduplication();
			faceIdkit->configuration->SetSimilarityThreshold(paramSimilarity);

			errorCode = faceIdkit->FindUser(templateData, std::get<2>(modelImage)[2], &userID, &score);
			error->CheckError(errorCode, error->medium);
			if (userID == 0 && errorCode == IENGINE_E_NOERROR) {
				errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
				error->CheckError(errorCode, error->medium);
				if (errorCode == IENGINE_E_NOERROR)
				{

					templateForMatch = NULL;
					lenghtMatch = 0;
					lastId = userID;
					BuildUserDatabase(modelImage, client, lastId);

				}
			}

		}
		else
		{
			errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
			error->CheckError(errorCode, error->medium);
			if (errorCode == IENGINE_E_NOERROR)
			{

				templateForMatch = NULL;
				lenghtMatch = 0;
				lastId = userID;
				BuildUserDatabase(modelImage, client, lastId);

			}
		}		
	}
	else {
		if (lastId != 0 && faceIdkit->configuration->GetIsConcatenateTemplates() == 1)
		{
			if (templateForMatch == NULL)
			{
				GetFaceTemplateUser();

			}
			MatchUsers(templateData, std::get<2>(modelImage)[2]);

		}

	}

	templateData = NULL;
}


void FaceIndentify::BuildUserDatabase(std::tuple<char*, 
	vector<unsigned char>, int*> modelImage, int client, int userId) {
	User* userForDatabase = new User();
	userForDatabase->SetStateUser(1);
	userForDatabase->SetMoldScore(std::get<2>(modelImage)[3]);
	userForDatabase->SetUserIdIFace(userId);
	userForDatabase->SetClient(client);
	userForDatabase->SetCropImageData(std::get<1>(modelImage));
	userForDatabase->SetMoldCropHeight(std::get<2>(modelImage)[1]);
	userForDatabase->SetMoldCropWidth(std::get<2>(modelImage)[0]);
	shootUser.on_next(userForDatabase);
}


void FaceIndentify::GetFaceTemplateUser() {
	unsigned char* templatePattern = faceIdkit->GetFaceTemplate(lastId, &lenghtMatch);
	templateForMatch = reinterpret_cast<const unsigned char*>(templatePattern);
	templatePattern = NULL;
}

void FaceIndentify::MatchUsers(const unsigned char* templateIn, int sizeIn) {
	int errorCode, score = -1;

	if (faceIdkit->configuration->GetConcatenateMode() == 0) {
		
		errorCode = faceIdkit->MatchUsers(templateForMatch,
			lenghtMatch, templateIn, sizeIn, &score);
		error->CheckError(errorCode, error->medium);
		if (errorCode == IENGINE_E_NOERROR) {
			if (score <= faceIdkit->configuration->GetConcatenationMaximumScore() &&
				score >= faceIdkit->configuration->GetConcatenationMinimumScore())
			{
				cout << "SCORE OK: " << score << endl;
				errorCode = faceIdkit->AddFaceTemplate(templateIn, sizeIn, lastId);
				error->CheckError(errorCode, error->medium);
			}
		}
	}
	else
	{
		errorCode = faceIdkit->AddFaceTemplate(templateIn, sizeIn, lastId);
		error->CheckError(errorCode, error->medium);
	}

}

void FaceIndentify::ImportUsers(std::tuple<char*, vector<unsigned char>, int*> modelImage, int client) {
	int errorCode, userID = -1, score = -1;
	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));

	if (faceIdkit->configuration->GetIsDeduplication() == 1) {			
		
		errorCode = faceIdkit->FindUser(templateData, std::get<2>(modelImage)[2], &userID, &score);
		error->CheckError(errorCode, error->medium);
		
		if (userID == 0 && errorCode == IENGINE_E_NOERROR) {
			errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
			error->CheckError(errorCode, error->medium);
			if (errorCode == IENGINE_E_NOERROR)
			{								
				BuildUserDatabase(modelImage, client, userID);

			}
		}

	}
	else
	{
		errorCode = faceIdkit->RegisterUser(templateData, std::get<2>(modelImage)[2], &userID);
		error->CheckError(errorCode, error->medium);
		if (errorCode == IENGINE_E_NOERROR)
		{
			
			BuildUserDatabase(modelImage, client, userID);

		}
	}
	templateData = NULL;
}

void FaceIndentify::ControlEntry(std::tuple<char*, vector<unsigned char>, int*> modelImage, int client) {
	int errorCode, userID = -1, score = -1;
	User* userForDatabase = new User();
	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));

	int paramSimilarity = faceIdkit->configuration->GetIdentificationThreshold();
	faceIdkit->configuration->SetSimilarityThreshold(paramSimilarity);

	errorCode = faceIdkit->FindUser(templateData, std::get<2>(modelImage)[2], &userID, &score);
	error->CheckError(errorCode, error->medium);

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
	templateForMatch = NULL;
}