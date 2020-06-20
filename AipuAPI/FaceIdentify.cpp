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
	error->CheckError(errorCode, error->medium);
	if (userID == 0 && configuration->GetIsRegister() && errorCode == IENGINE_E_NOERROR)
	{

		errorCode = addUserToDatabase(templateData, std::get<2>(modelImage)[2], &userID);
		error->CheckError(errorCode, error->medium);
		if (errorCode == IENGINE_E_NOERROR) {
			userForDatabase->SetIsNew(true);
		}
		userForDatabase->SetIsNew(true);
	}


	if (errorCode == IENGINE_E_NOERROR && userID != 0 ) {
		/*lastUser = userID;
		countRepeatFrame = 0;*/
		if (!userForDatabase->GetIsNew())
		{
			countRepeatUser++;
		}
		else
		{
			countNewUser++;
		}
		userForDatabase->SetUserIdIFace(userID);
		userForDatabase->SetClient(client);
		userForDatabase->SetCropImageData(std::get<1>(modelImage));
		userForDatabase->SetMoldCropHeight(std::get<2>(modelImage)[1]);
		//userForDatabase->SetMoldCropLength(modelImage->GetMoldCropLength());
		userForDatabase->SetMoldCropWidth(std::get<2>(modelImage)[0]);
		
		shootUser.on_next(userForDatabase);
	}


	/*else
	{
		countRepeatFrame++;
		if (countRepeatFrame > 1)
		{
			lastUser = 0;
			countRepeatFrame = 0;
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
