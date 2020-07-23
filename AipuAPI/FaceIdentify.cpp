#include "FaceIdentify.h"

FaceIndentify::FaceIndentify()
{
}

FaceIndentify::~FaceIndentify()
{
	delete configuration;
	delete error;
	delete format;
}

void FaceIndentify::LoadConnection() {
	/*int errorCode;
	std::string str = "iengine.db";
	const char *cstr = str.c_str();	*/

	SetParamsIdentify();
	/*errorCode = connectToDatabase(cstr);
	cout << "connectToDatabase returns " << errorCode << endl;	*/

	/*errorCode = IEngine_InitModule();
	error->CheckError(errorCode, error->medium);
	errorCode = IEngine_SetParameter(CFG_IDENTIFICATION_SPEED, 4);
	error->CheckError(errorCode, error->medium);
	errorCode = IEngine_SetParameter(CFG_MAX_ROTATION, 90);
	error->CheckError(errorCode, error->medium);
	errorCode = IEngine_Connect(cstr);
	error->CheckError(errorCode, error->medium);*/


	/*errorCode = IEngine_ClearDatabase();
	error->CheckError(errorCode, error->medium);*/
}

void FaceIndentify::SetParamsIdentify() {
	int errorCode;
	//std::string paramsIdentify = configuration->ParseMapToJSONForIdentify();	
	/*std::string paramsIdentify = format->FormatString("{\"A_IdentificationSpeed\": %d, \"A_SimilarityThreshold\": %d, \"A_BestMatchedCandidates\": %d}",
		configuration->GetIdentificationSpeed(), 
		configuration->GetSimilarityThreshold(),
		configuration->GetBestMatchedCandidates());*/

	string paramsIdentify = format->FormatString("{\"A_IdentificationSpeed\": %d, ",
		configuration->GetIdentificationSpeed());
	paramsIdentify += format->FormatString("\"A_MinEyeDist\": %d, ",
		configuration->GetMinEyeDistance());
	paramsIdentify += format->FormatString("\"A_MaxEyeDist\": %d, ",
		configuration->GetMaxEyeDistance());
	paramsIdentify += format->FormatString("\"A_FaceDetectionForced\": %d, ",
		configuration->GetFaceDetectionForced());
	paramsIdentify += format->FormatString("\"A_SimilarityThreshold\": %d, ",
		configuration->GetSimilarityThreshold());
	paramsIdentify += format->FormatString("\"A_BestMatchedCandidates\": %d}",
		configuration->GetBestMatchedCandidates());

	/*std::string paramsIdentify = format->FormatString("{\"A_IdentificationSpeed\": %d, \"A_MinEyeDist\": %d, \"A_MaxEyeDist\": %d, \"A_FaceDetectionForced\": %d}",
		configuration->GetIdentificationSpeed(),
		configuration->GetMinEyeDistance(),
		configuration->GetMaxEyeDistance(), configuration->GetFaceDetectionForced());*/

	errorCode = setBiometricParameters(paramsIdentify.c_str());
	
	cout << "setBiometricParameters returns " << errorCode << endl;
	//error->CheckError(errorCode, error->medium);
	
}

void FaceIndentify::EnrollUser(std::tuple<char*, 
	vector<unsigned char>, int*> modelImage, int client) {

	//flagEnroll = true;
	int errorCode, userID, score;
	//IENGINE_USER user = IEngine_InitUser();
	User* userForDatabase = new User();
	const unsigned char* templateData = reinterpret_cast<const unsigned char*>(std::get<0>(modelImage));

	errorCode = identify(templateData, std::get<2>(modelImage)[2], &userID, &score);
	//cout << "IENGINE_E_ERROR: " << errorCode << " userID: " << userID << endl;
	error->CheckError(errorCode, error->medium);
	/*if (userID == 0 && configuration->GetIsRegister() && errorCode == IENGINE_E_NOERROR)
	{

		errorCode = addUserToDatabase(templateData, std::get<2>(modelImage)[2], &userID);
		error->CheckError(errorCode, error->medium);
		if (errorCode == IENGINE_E_NOERROR) {
			userForDatabase->SetStateUser(1);
		}
		userForDatabase->SetStateUser(1);
	}*/

	if (userID == 0  && errorCode == IENGINE_E_NOERROR)
	{

		errorCode = addUserToDatabase(templateData, std::get<2>(modelImage)[2], &userID);
		error->CheckError(errorCode, error->medium);
		if (configuration->GetIsRegister() == 1)
		{
			userForDatabase->SetStateUser(1);
		}
		else
		{
			userForDatabase->SetStateUser(3);
		}
		
	}

	if (errorCode == IENGINE_E_NOERROR && userID != 0 ) {
		
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

		if (userForDatabase->GetStateUser() == 3 && lastUserUnidentified != userID) {
			
			RemoveUnidentified();
			lastUserUnidentified = userID;
			userForDatabase->SetUserIdIFace(userID);
			userForDatabase->SetClient(client);
			userForDatabase->SetCropImageData(std::get<1>(modelImage));
			userForDatabase->SetMoldCropHeight(std::get<2>(modelImage)[1]);
			userForDatabase->SetMoldCropWidth(std::get<2>(modelImage)[0]);
			shootUser.on_next(userForDatabase);
		}

		if ((userForDatabase->GetStateUser() == 2 || 
			userForDatabase->GetStateUser() == 1) && lastUserUnidentified != userID)
		{
			
			userForDatabase->SetUserIdIFace(userID);
			userForDatabase->SetClient(client);
			userForDatabase->SetCropImageData(std::get<1>(modelImage));
			userForDatabase->SetMoldCropHeight(std::get<2>(modelImage)[1]);
			userForDatabase->SetMoldCropWidth(std::get<2>(modelImage)[0]);
			shootUser.on_next(userForDatabase);
		}
					
	}
	

	/*else if(errorCode == IENGINE_E_NOERROR && userID == 0)
	{
		if (countLastUserUnidentified == 0)
		{
			userForDatabase->SetStateUser(3);
			userForDatabase->SetUserIdIFace(countUserNothing);
			userForDatabase->SetClient(client);
			userForDatabase->SetMoldScore(std::get<2>(modelImage)[3]);
			userForDatabase->SetCropImageData(std::get<1>(modelImage));
			userForDatabase->SetMoldCropHeight(std::get<2>(modelImage)[1]);
			userForDatabase->SetMoldCropWidth(std::get<2>(modelImage)[0]);
			countUserNothing--;
			shootUser.on_next(userForDatabase);
		}
		countLastUserUnidentified++;
		if (countLastUserUnidentified == 7)
		{
			countLastUserUnidentified = 0;
		}
	}*/

	//delete userForDatabase;
	//flagEnroll = false;
	templateData = NULL;

	//errorCode = IEngine_AddFaceTemplate(user, templateData, modelImage->GetMoldSize());
	//if (errorCode == IENGINE_E_NOERROR) {
	//	User* userForDatabase = new User();

	//	errorCode = IEngine_Connect("iengine.db");
	//	error->CheckError(errorCode, error->medium);

	//	errorCode = IEngine_FindUser(user, &userID, &score);
	//	if (userID == 0 && isRegister)
	//	{

	//		errorCode = IEngine_RegisterUser(user, &userID);
	//		error->CheckError(errorCode, error->medium);
	//		if (errorCode == IENGINE_E_NOERROR) {
	//			userForDatabase->SetIsNew(true);
	//		}
	//	}
	//	if (errorCode == IENGINE_E_NOERROR && userID != 0) {
	//		/*userForDatabase->SetUserIdIFace(userID);
	//		userForDatabase->SetPathImageTemp(modelImage->GetPathImage());
	//		userForDatabase->SetClient(modelImage->GetIdClient());*/
	//		if (!userForDatabase->GetIsNew())
	//		{
	//			countRepeatUser++;
	//		}
	//		userForDatabase->SetUserIdIFace(userID);
	//		userForDatabase->SetClient(modelImage->GetIdClient());
	//		userForDatabase->SetCropImageData(modelImage->GetCropImageData());
	//		userForDatabase->SetMoldCropHeight(modelImage->GetMoldCropHeight());
	//		userForDatabase->SetMoldCropLength(modelImage->GetMoldCropLength());
	//		userForDatabase->SetMoldCropWidth(modelImage->GetMoldCropWidth());
	//		shootUser.on_next(userForDatabase);
	//	}
	//}
	//else {
	//	error->CheckError(errorCode, error->medium);
	//}

	//errorCode = IEngine_FreeUser(user);
	//error->CheckError(errorCode, error->medium);
}

void FaceIndentify::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
		shootError.on_next(either);
	});
}

void FaceIndentify::RemoveUnidentified() {
	
	if (lastUserUnidentified != 0)
	{
		if (removeUser(lastUserUnidentified) == 0) {
			cout << "REMOVE UNIDENTIFIED OK" << endl;
		}
		else
		{
			cout << "REMOVE UNIDENTIFIED ERROR" << endl;
		}
	}
}