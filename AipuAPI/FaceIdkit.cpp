#include "FaceIdkit.h"

string connectDatabase = "iengine.db";

FaceIdkit::FaceIdkit()
{
}

FaceIdkit::~FaceIdkit()
{

}

int FaceIdkit::InitLibrary() {
	int errorCode, userLimit;
	const char* vers_string = IEngine_GetProductString();
	std::cout << "Product string is: " << vers_string << std::endl;	
	errorCode = IEngine_InitModule();	
	errorCode = IEngine_GetUserLimit(&userLimit);	
	std::cout << "Maximum permitted number of users in the database: " << userLimit << std::endl;
	errorCode = IEngine_SetParameter(CFG_LOG_LEVEL, 0);
	errorCode = IEngine_InitClient(connectDatabase.c_str());
	return errorCode;

}

int FaceIdkit::TerminateLibrary() {
	
	return IEngine_TerminateModule();
	
}

void FaceIdkit::SetParameters() {
	int errorCode;
	errorCode = IEngine_SetParameter(CFG_IDENTIFICATION_SPEED, 
		configuration->GetIdentificationSpeed());	
	error->CheckError(errorCode, error->medium);
	
	errorCode = IEngine_SetParameter(CFG_IFACE_DETECT_FORCED, 
		configuration->GetFaceDetectionForced());
	error->CheckError(errorCode, error->medium);
	
	errorCode = IEngine_SetParameter(CFG_SIMILARITY_THRESHOLD,
		configuration->GetSimilarityThreshold());
	error->CheckError(errorCode, error->medium);
	
	errorCode = IEngine_SetParameter(CFG_BEST_CANDIDATES_COUNT,
		configuration->GetBestMatchedCandidates());
	error->CheckError(errorCode, error->medium);
	
	errorCode = IEngine_SetParameter(CFG_IFACE_IGNORE_MULTIPLE_FACES,
		configuration->GetIgnoreMultipleFaces());
	error->CheckError(errorCode, error->medium);
	
	errorCode = IEngine_SetParameter(CFG_IFACE_DETECTION_MODE,
		configuration->GetFaceDetectionMode());
	error->CheckError(errorCode, error->medium);
	
	errorCode = IEngine_SetParameter(CFG_IFACE_EXTRACTION_MODE,
		configuration->GetFaceExtractionMode());
	error->CheckError(errorCode, error->medium);
	
	errorCode = IEngine_SetParameter(CFG_IFACE_DETECTION_THRESHOLD,
		configuration->GetDetectionThreshold());
	error->CheckError(errorCode, error->medium);
	
}

void FaceIdkit::Connection() {
	int errorCode;
	
	handleConnect = IEngine_InitConnection();

	if (handleConnect != NULL)
	{
		errorCode = IEngine_SelectConnection(handleConnect);
		if (errorCode == IENGINE_E_NOERROR) {
			SetParameters();
			errorCode = IEngine_Connect(connectDatabase.c_str());
			user = IEngine_InitUser();
			userAux = IEngine_InitUser();
			userAuxMatch = IEngine_InitUser();
			error->CheckError(errorCode, error->medium);
			
		}
						
	}
	else {
		cout << "IEngine_InitConnection ERROR" << endl;
	}	
}

void FaceIdkit::CloseConnection() {
	int errorCode;
	errorCode = IEngine_FreeUser(user);
	errorCode = IEngine_FreeUser(userAux);
	errorCode = IEngine_FreeUser(userAuxMatch);
	error->CheckError(errorCode, error->medium);
	errorCode = IEngine_CloseConnection(handleConnect);
	error->CheckError(errorCode, error->medium);
}

int FaceIdkit::GetSimilarityThreshold() {
	int errorCode = IENGINE_E_NOERROR, paramSimilarity = -1;
	
	errorCode = IEngine_SelectConnection(handleConnect);
	error->CheckError(errorCode, error->medium);

	if (errorCode == IENGINE_E_NOERROR) {
		errorCode = IEngine_GetParameter(CFG_SIMILARITY_THRESHOLD, &paramSimilarity);
		error->CheckError(errorCode, error->medium);
	}
	return paramSimilarity;
}

int FaceIdkit::FindUser(const unsigned char* templateModel,
	int size, int* userId, int* score, string* tracer) {
	int errorCode = IENGINE_E_NOERROR;
	int errorClear;	

	errorCode = IEngine_AddFaceTemplate(userAux, templateModel, size);
	if (errorCode == IENGINE_E_NOERROR) {
				
		errorCode = IEngine_SelectConnection(handleConnect);
		error->CheckError(errorCode, error->medium);
		if (errorCode == IENGINE_E_NOERROR) {			
			int bestCandidatesCount = configuration->GetBestMatchedCandidates();			
			/*int paramSimilarity = -1;
			errorCode = IEngine_GetParameter(CFG_SIMILARITY_THRESHOLD, &paramSimilarity);			
			*tracer += " PARAM SIMILARITY THRESHOLD: " + to_string(paramSimilarity) + ", ";*/

			errorCode = IEngine_GetParameter(CFG_BEST_CANDIDATES_COUNT, &bestCandidatesCount);
			memset(userId, sizeof(int) * bestCandidatesCount, 0);
			memset(score, sizeof(int) * bestCandidatesCount, 0);

			errorCode = IEngine_FindUser(userAux, userId, score);
			error->CheckError(errorCode, error->medium);
			
			for (int i = 0; i < bestCandidatesCount; i++)
			{
				*tracer += "(" + to_string(userId[i]) + "-" + to_string(score[i]) + ") ";
				
			}
			
		}
		
	}		
	errorClear = IEngine_ClearUser(userAux);
	error->CheckError(errorClear, error->medium);
			
	return errorCode;
}

void FaceIdkit::ClearUser() {
	int errorCode = IENGINE_E_NOERROR;
	errorCode = IEngine_ClearUser(user);
	error->CheckError(errorCode, error->medium);

}

int FaceIdkit::RegisterUser(const unsigned char* templateModel,
	int size, int* userId) {
	int errorCode = IENGINE_E_NOERROR;
	int errorClear;
	
	errorCode = IEngine_AddFaceTemplate(user, templateModel, size);
	if (errorCode == IENGINE_E_NOERROR) {
		errorCode = IEngine_SelectConnection(handleConnect);
		error->CheckError(errorCode, error->medium);
		if (errorCode == IENGINE_E_NOERROR) {
			errorCode = IEngine_RegisterUser(user, userId);
			error->CheckError(errorCode, error->medium);			
		}
		
	}
	errorClear = IEngine_ClearUser(user);
	error->CheckError(errorClear, error->medium);
			
	return errorCode;

}

int FaceIdkit::RemoveUser(int userId) {
	int errorCode = IENGINE_E_NOERROR;
	
	errorCode = IEngine_SelectConnection(handleConnect);
	error->CheckError(errorCode, error->medium);
	if (errorCode == IENGINE_E_NOERROR) {
		errorCode = IEngine_RemoveUser(userId);
		error->CheckError(errorCode, error->medium);
	}
	
	return errorCode;
}

int FaceIdkit::MatchUsers(const unsigned char* templateModelOne,
	int sizeOne, const unsigned char* templateModelTwo, int sizeTwo, int* score) {
	int errorCode = IENGINE_E_NOERROR, errorClear;
	
	errorCode = IEngine_AddFaceTemplate(userAuxMatch, templateModelOne, sizeOne);
	error->CheckError(errorCode, error->medium);
	if (errorCode == IENGINE_E_NOERROR) {
		
		errorCode = IEngine_AddFaceTemplate(userAux, templateModelTwo, sizeTwo);
		error->CheckError(errorCode, error->medium);
		if (errorCode == IENGINE_E_NOERROR) {
			
			errorCode = IEngine_SelectConnection(handleConnect);
			error->CheckError(errorCode, error->medium);
			if (errorCode == IENGINE_E_NOERROR) {								
				errorCode = IEngine_MatchUsersModalities(userAuxMatch, userAux,
					IENGINE_MATCHING_MODALITY::MODALITY_FACE, score);
				error->CheckError(errorCode, error->medium);
				
			}
		}
	}
	errorClear = IEngine_ClearUser(userAuxMatch);
	errorClear = IEngine_ClearUser(userAux);
	return errorCode;

}

int FaceIdkit::AddFaceTemplate(const unsigned char* templateModel, 
	int size, int userID) {
	int errorCode, errorClear;
	errorCode = IEngine_SelectConnection(handleConnect);
	error->CheckError(errorCode, error->medium);
	if (errorCode == IENGINE_E_NOERROR) {
		
		errorCode = IEngine_GetUser(user, userID);
		error->CheckError(errorCode, error->medium);
		if (errorCode == IENGINE_E_NOERROR) {
			errorCode = IEngine_AddFaceTemplate(user, templateModel, size);
			cout << "ADD TEMPLATE ERROR: " << errorCode << endl;
		}
	}
	errorClear = IEngine_ClearUser(user);
	return errorCode;
}

int FaceIdkit::AddFaceTemplate(const unsigned char* templateModel, int size) {
	int errorCode;
	errorCode = IEngine_SelectConnection(handleConnect);
	error->CheckError(errorCode, error->medium);
	if (errorCode == IENGINE_E_NOERROR) {
		
		errorCode = IEngine_AddFaceTemplate(user, templateModel, size);		
		
	}
	
	return errorCode;
}

int FaceIdkit::RegisterUserWithData(const unsigned char* templateModel, int size,
	const unsigned char* userData, int dataLength, int* userId) {
	int errorCode, errorClear;
	errorCode = IEngine_AddFaceTemplate(user, templateModel, size);
	error->CheckError(errorCode, error->medium);
	if (errorCode == IENGINE_E_NOERROR)
	{
		errorCode = IEngine_SetCustomData(user, userData, dataLength);
		error->CheckError(errorCode, error->medium);
		if (errorCode == IENGINE_E_NOERROR) {
			errorCode = IEngine_SelectConnection(handleConnect);
			error->CheckError(errorCode, error->medium);
			if (errorCode == IENGINE_E_NOERROR) {
				errorCode = IEngine_RegisterUser(user, userId);
			}
		}
	}
	errorClear = IEngine_ClearUser(user);
	return errorCode;
}

unsigned char* FaceIdkit::GetFaceTemplate(int userId, int* lenght, int index) {
	int errorCode, errorClear;
	errorCode = IEngine_SelectConnection(handleConnect);
	error->CheckError(errorCode, error->medium);
	if (errorCode == IENGINE_E_NOERROR) {
		errorCode = IEngine_GetUser(userAux, userId);
		error->CheckError(errorCode, error->medium);
		if (errorCode == IENGINE_E_NOERROR) {
			errorCode = IEngine_GetFaceTemplate(userAux, index, NULL, lenght);
			error->CheckError(errorCode, error->medium);			
			unsigned char*  templateReturn = new unsigned char[*lenght];
			errorCode = IEngine_GetFaceTemplate(userAux, index, templateReturn, lenght);
			error->CheckError(errorCode, error->medium);
			
			errorClear = IEngine_ClearUser(userAux);
			return templateReturn;
			
		}
		errorClear = IEngine_ClearUser(userAux);
	}
		
	return NULL;
		
}