#include "FaceModel.h"

FaceModel::FaceModel()
{
	ObserverError();
}

FaceModel::~FaceModel()
{
	Terminate();
	delete configuration;
	delete error;
}

void FaceModel::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([this](Either* either) {

		shootError.on_next(either);
	});

	auto observerErrorConfiguration = configuration->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionErrorConfiguration = observerErrorConfiguration.subscribe([this](Either* either) {

		shootError.on_next(either);
	});

}

void FaceModel::Terminate() {
	int errorCode;
	TerminateHandle();
	errorCode = IFACE_Terminate();
	error->CheckError(errorCode, error->less);

}

void FaceModel::TerminateHandle() {
	int errorCode;
	errorCode = IFACE_ReleaseEntity(faceHandlerGlobal);
	error->CheckError(errorCode, error->medium);
}


int FaceModel::DetectByBatch(void* facesDetected[BATCH_TOTAL_SIZE],
	std::vector<std::vector<unsigned char>> bufferOfImagesBatch,
	std::vector<int> clients) {
	int errorCode, countFacesDetected = 0;
	void* faceHandler;
	
	errorCode = IFACE_CreateFaceHandler(&faceHandler);
	error->CheckError(errorCode, error->medium);

	if (configuration->GetModeDetect() == 1)
	{
		errorCode = IFACE_SetParam(faceHandler,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_BALANCED);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetModeDetect() == 2)
	{
		errorCode = IFACE_SetParam(faceHandler,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_FAST);
		error->CheckError(errorCode, error->medium);
	}


	for (int i = 0; i < BATCH_TOTAL_SIZE; i++)
	{

		errorCode = IFACE_CreateFace(&(facesDetected[i]));
		error->CheckError(errorCode, error->medium);
	}

	for (int i = 0; i < BATCH_SIZE; i++)
	{
		int lenght, widthFace, heightFace;

		vector<unsigned char> buffer = bufferOfImagesBatch[i];
		const char* imgData = reinterpret_cast<const char*> (&buffer[0]);

		if (imgData != NULL) {
			errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), &widthFace,
				&heightFace, &lenght, NULL);
			if (errorCode == IFACE_OK)
			{
				int maxFaces = configuration->GetMaxDetect();
				unsigned char* rawImage = new unsigned char[lenght];
				errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), &widthFace,
					&heightFace, &lenght, rawImage);

				error->CheckError(errorCode, error->medium);
				void** faceTemp = new void*[maxFaces];

				for (int j = 0; j < maxFaces; j++) {
					errorCode = IFACE_CreateFace(&(faceTemp[j]));
					error->CheckError(errorCode, error->medium);
				}

				int detectedFaces = configuration->GetMaxDetect();

				errorCode = IFACE_DetectFaces(rawImage, widthFace, heightFace,
					(float)configuration->GetMinEyeDistance(), (float)configuration->GetMaxEyeDistance(),
					faceHandler, &detectedFaces, faceTemp);
				delete[] rawImage;
				error->CheckError(errorCode, error->medium);

				if (detectedFaces != EMPTY_FACE)
				{
					for (int j = 0; j < detectedFaces; j++) {
						float rightEyeX, rightEyeY, leftEyeX, leftEyeY;
						float faceConfidence;
						void* face = faceTemp[j];
						errorCode = IFACE_GetFaceBasicInfo(face, faceHandler,
							&rightEyeX, &rightEyeY, &leftEyeX, &leftEyeY, &faceConfidence);
						error->CheckError(errorCode, error->medium);
						cout << "CONFIDENCE: " << faceConfidence << endl;
						if (faceConfidence > configuration->GetPrecision() &&
							countFacesDetected < BATCH_TOTAL_SIZE)
						{
							cout << "GREATER OR EQUAL ACCURACY .." << configuration->GetPrecision() << endl;
							errorCode = IFACE_CloneEntity(face, facesDetected[countFacesDetected]);
							error->CheckError(errorCode, error->medium);

							Molded* model = new Molded();
							model->SetIdClient(clients[i]);
							FaceCropImage(facesDetected[countFacesDetected], model);
							modelsDetected.push_back(model);
							countFacesDetected++;

						}
						else {
							countLowScore++;
						}
					}

				}
				else {
					countNotDetect++;
				}

				for (int j = 0; j < maxFaces; j++) {
					errorCode = IFACE_ReleaseEntity(faceTemp[j]);
					error->CheckError(errorCode, error->medium);
				}

			}
			else {
				error->CheckError(errorCode, error->medium);
			}

		}


	}
	errorCode = IFACE_ReleaseEntity(faceHandler);
	error->CheckError(errorCode, error->medium);

	return countFacesDetected;
}
//void FaceModel::GetFaceCropRectangle(void* face) {
//	float cropRect[8];
//	int errorCode;
//	void* faceHandler;
//	errorCode = IFACE_CreateFaceHandler(&faceHandler);
//	error->CheckError(errorCode, error->medium);
//
//	errorCode = IFACE_GetFaceCropRectangle(face, faceHandler,
//		IFACE_FACE_CROP_METHOD_FULL_FRONTAL, cropRect);
//	error->CheckError(errorCode, error->medium);
//	if (errorCode == IFACE_OK) {
//		shootCoordinates.on_next(cropRect);
//	}
//	errorCode = IFACE_ReleaseEntity(faceHandler);
//	error->CheckError(errorCode, error->medium);
//}

void FaceModel::FaceCropImage(void* face, Molded* model) {
	int cropWidth, cropHeight, cropLength, errorCode;
	/*void* faceHandler;

	errorCode = IFACE_CreateFaceHandler(&faceHandler);
	error->CheckError(errorCode, error->medium);*/


	errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal,
		IFACE_FACE_CROP_METHOD_FULL_FRONTAL,
		&cropWidth, &cropHeight, &cropLength, NULL);
	error->CheckError(errorCode, error->medium);
	unsigned char* cropImageData = new unsigned char[cropLength];
	errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal, 
		IFACE_FACE_CROP_METHOD_FULL_FRONTAL,
		&cropWidth, &cropHeight, &cropLength, cropImageData);
	error->CheckError(errorCode, error->medium);

	if (errorCode == IFACE_OK) {
		model->SetMoldCropLength(cropLength);
		model->SetCropImageData(cropImageData);
		model->SetMoldCropWidth(cropWidth);
		model->SetMoldCropHeight(cropHeight);
		
	}

	/*errorCode = IFACE_ReleaseEntity(faceHandler);
	error->CheckError(errorCode, error->medium);*/
	delete[] cropImageData;
}

int FaceModel::ModelByBatch(std::vector<std::vector<unsigned char>> bufferOfImagesBatch,
	std::vector<int> clients) {
	int errorCode;
	void* facesDetected[BATCH_TOTAL_SIZE];
	int countFacesDetected = DetectByBatch(facesDetected, bufferOfImagesBatch, clients);
	if (countFacesDetected != 0)
	{
		GetBatchModels(countFacesDetected, facesDetected);
	}

	for (int i = 0; i < BATCH_TOTAL_SIZE; i++)
	{
		errorCode = IFACE_ReleaseEntity(facesDetected[i]);
		error->CheckError(errorCode, error->medium);
	}
	return countFacesDetected;
}

void FaceModel::GetBatchModels(int countFacesDetected, void* facesDetected[BATCH_TOTAL_SIZE]) {
	int errorCode;
	int templateBatchDataSize;
	void** cloneFacesDetected = new void*[countFacesDetected];
	char** batchTemplates = new char*[countFacesDetected];
	void* faceHandler;

	errorCode = IFACE_CreateFaceHandler(&faceHandler);
	error->CheckError(errorCode, error->medium);

	for (int i = 0; i < countFacesDetected; i++)
	{
		errorCode = IFACE_CreateFace(&(cloneFacesDetected[i]));
		error->CheckError(errorCode, error->medium);
	}

	for (int i = 0; i < countFacesDetected; i++)
	{
		errorCode = IFACE_CloneEntity(facesDetected[i], cloneFacesDetected[i]);
		error->CheckError(errorCode, error->medium);
	}

	errorCode = IFACE_CreateTemplateBatch(faceHandler, countFacesDetected, cloneFacesDetected,
		&templateBatchDataSize, NULL);
	error->CheckError(errorCode, error->medium);

	for (int i = 0; i < countFacesDetected; ++i)
	{

		batchTemplates[i] = new char[templateBatchDataSize];
	}

	errorCode = IFACE_CreateTemplateBatch(faceHandler, countFacesDetected, cloneFacesDetected,
		&templateBatchDataSize, batchTemplates);

	error->CheckError(errorCode, error->medium);

	for (int i = 0; i < countFacesDetected; i++)
	{
		modelsDetected[i]->SetMoldImage(batchTemplates[i]);
		modelsDetected[i]->SetMoldSize(templateBatchDataSize);		

		//templateOut.on_next(modelsDetected[i]);

	}

	for (int i = 0; i < countFacesDetected; ++i)
	{
		delete batchTemplates[i];
	}
	for (int i = 0; i < countFacesDetected; i++)
	{
		errorCode = IFACE_ReleaseEntity(cloneFacesDetected[i]);
		error->CheckError(errorCode, error->medium);
	}

	errorCode = IFACE_ReleaseEntity(faceHandler);
	error->CheckError(errorCode, error->medium);	

	modelsDetected.clear();
}

unsigned char* FaceModel::LoadImage(vector<unsigned char> buffer, int *width, int *height) {
	int lenght, errorCode;
	const char* imgData = reinterpret_cast<const char*> (&buffer[0]);
	if (imgData == NULL) {
		return NULL;
	}

	errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
		height, &lenght, NULL);
	if (errorCode != IFACE_OK) {
		error->CheckError(errorCode, error->medium);
		return NULL;
	}
	unsigned char* rawImage = new unsigned char[lenght];
	errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
		height, &lenght, rawImage);
	if (errorCode != IFACE_OK) {
		error->CheckError(errorCode, error->medium);
		return NULL;
	}

	return rawImage;

}

//void FaceModel::FastOnlyDetect(vector<unsigned char> buffer) {
//	int lenght, width, height, errorCode;
//	const char* imgData = reinterpret_cast<const char*> (&buffer[0]);
//	if (imgData != NULL) {
//		errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), &width,
//			&height, &lenght, NULL);
//		if (errorCode == IFACE_OK)
//		{
//
//			unsigned char* rawImage = new unsigned char[lenght];
//			errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), &width,
//				&height, &lenght, rawImage);
//			if (errorCode != IFACE_OK)
//			{
//				error->CheckError(errorCode, error->medium);
//			}
//			else
//			{
//				FastDetect(rawImage, width, height);
//			}
//			delete rawImage;
//		}
//		else {
//			error->CheckError(errorCode, error->medium);
//		}
//	}
//
//}


int FaceModel::ModelOneToOne(vector<unsigned char> buffer, int client) {
	int lenght, width, height, errorCode, templates = 0;
	const char* imgData = reinterpret_cast<const char*> (&buffer[0]);

	if (imgData != NULL) {
		errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), &width,
			&height, &lenght, NULL);
		if (errorCode == IFACE_OK)
		{

			unsigned char* rawImage = new unsigned char[lenght];
			errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), &width,
				&height, &lenght, rawImage);
			if (errorCode != IFACE_OK)
			{
				error->CheckError(errorCode, error->medium);
			}
			else
			{
				templates = GetOneModel(rawImage, width, height, client);
			}
			delete[] rawImage;
		}
		else {
			error->CheckError(errorCode, error->medium);
		}
	}

	return templates;
}

void FaceModel::RecognitionFaceFiles(string file, int client) {
	int lenght, width, height, templates;
	
	isFinishLoadFiles = false;
	
	/*string nameFile = configuration->GetNameDirectory() + "/" + file;
	ifstream data(nameFile, std::ifstream::in);
	if (data.good()) {
		string row;
		while (data >> row && !row.empty()) {
			unsigned char* rawImage = LoadFileImage(row, &width, &height, &lenght);
			if (rawImage != NULL) {
				templates = GetOneModel(rawImage, width, height, client);
			}
			delete rawImage;
		}
		data.close();
	}*/

	unsigned char* rawImage = LoadFileImage(file, &width, &height, &lenght);
	if (rawImage != NULL) {
		templates = GetOneModel(rawImage, width, height, client);
	}
	delete[] rawImage;

	//memset(rawImage, 0, lenght);
	isFinishLoadFiles = true;
}

unsigned char* FaceModel::LoadFileImage(string image, int *width, int *height, int *length)
{
	int errorCode = IFACE_LoadImage(image.c_str(), width, height, length, NULL);
	if (errorCode != IFACE_OK)
	{
		
		error->CheckError(errorCode, error->medium);
		return NULL;
	}

	unsigned char *imageData = new unsigned char[*length];
	errorCode = IFACE_LoadImage(image.c_str(), width, height, length, imageData);
	if (errorCode != IFACE_OK)
	{
		
		error->CheckError(errorCode, error->medium);
		return NULL;
	}

	return imageData;
}

void FaceModel::CreateTemplate(void* face, Molded* model, int client) {
	int errorCode, majorVersion, minorVersion, quality;
	int templateSize;
	/*void* faceHandler;

	errorCode = IFACE_CreateFaceHandler(&faceHandler);
	error->CheckError(errorCode, error->medium);*/
	errorCode = IFACE_CreateTemplate(face, faceHandlerGlobal, 0, &templateSize, NULL);
	if (errorCode == IFACE_OK) {
		char* templateData = new char[templateSize];
		errorCode = IFACE_CreateTemplate(face, faceHandlerGlobal, 0,
			&templateSize, templateData);
		if (errorCode != IFACE_OK) {
			error->CheckError(errorCode, error->medium);
		}
		else
		{
			errorCode = IFACE_GetTemplateInfo(faceHandlerGlobal,
				templateData, &majorVersion, &minorVersion, &quality);
			if (quality > configuration->GetQualityModel()) {
				/*model->SetMoldImage(templateData);
				model->SetMoldSize(templateSize);
				model->SetIdClient(client);*/

				int sizeImage[4];
				sizeImage[0] = model->GetMoldCropWidth();
				sizeImage[1] = model->GetMoldCropHeight();
				sizeImage[2] = templateSize;
				sizeImage[3] = quality;

				auto tupleTemplateFace = std::make_tuple(templateData,
					model->GetCropImageData(), sizeImage);
				templateOut.on_next(tupleTemplateFace);
			}
			
		}
		delete[] templateData;
	}
	else {
		error->CheckError(errorCode, error->medium);
	}
	

}
void FaceModel::InitHandle() {
	int errorCode;

	errorCode = IFACE_CreateFaceHandler(&faceHandlerGlobal);
	error->CheckError(errorCode, error->medium);

	if (configuration->GetExtractionMode() == 0) {
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_ACCURATE);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetExtractionMode() == 1) {
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_FAST);
		error->CheckError(errorCode, error->medium);
	}

	if (configuration->GetModeDetect() == 0)
	{
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_BALANCED);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetModeDetect() == 1)
	{
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_FAST);
		error->CheckError(errorCode, error->medium);
	}
}

int FaceModel::GetOneModel(unsigned char* rawImage,
	int width, int height, int client) {
	int maxFaces = configuration->GetMaxDetect();
	int detectedFaces = configuration->GetMaxDetect();
	int errorCode;
	void** faceTemp = new void*[maxFaces];
	//void* faceHandler;

	/*errorCode = IFACE_CreateFaceHandler(&faceHandler);
	error->CheckError(errorCode, error->medium);	

	if (configuration->GetExtractionMode() == 0) {
		errorCode = IFACE_SetParam(faceHandler,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_ACCURATE);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetExtractionMode() == 1) {
		errorCode = IFACE_SetParam(faceHandler,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_FAST);
		error->CheckError(errorCode, error->medium);
	}

	if (configuration->GetModeDetect() == 1)
	{
		errorCode = IFACE_SetParam(faceHandler,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_BALANCED);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetModeDetect() == 2)
	{
		errorCode = IFACE_SetParam(faceHandler,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_FAST);
		error->CheckError(errorCode, error->medium);
	}*/

	for (int i = 0; i < maxFaces; i++) {
		errorCode = IFACE_CreateFace(&(faceTemp[i]));
		error->CheckError(errorCode, error->medium);
	}

	errorCode = IFACE_DetectFaces(rawImage, width, height,
		(float)configuration->GetMinEyeDistance(), (float)configuration->GetMaxEyeDistance(),
		faceHandlerGlobal, &detectedFaces, faceTemp);

	if (errorCode == IFACE_OK) {

		if (detectedFaces != EMPTY_FACE)
		{
			for (int i = 0; i < detectedFaces; i++) {
				float rightEyeX, rightEyeY, leftEyeX, leftEyeY;
				float faceConfidence;
				void* face = faceTemp[i];
				                                                                    
				errorCode = IFACE_GetFaceBasicInfo(face, faceHandlerGlobal,
					&rightEyeX, &rightEyeY, &leftEyeX, &leftEyeY, &faceConfidence);
				error->CheckError(errorCode, error->medium);
				cout << "CONFIDENCE: " << faceConfidence << endl;
				if (faceConfidence > configuration->GetPrecision())
				{
					cout << "GREATER OR EQUAL ACCURACY .." << configuration->GetPrecision() << endl;
					Molded* model = new Molded();
					FaceCropImage(face, model);
					CreateTemplate(face, model, client);
					delete model;
					countDetect++;
				}
				else {
					countLowScore++;
				}

			}
		}
		else {
			countNotDetect++;
		}
	}
	else {
		error->CheckError(errorCode, error->medium);
		countErrorDetect++;
	}

	for (int i = 0; i < maxFaces; i++) {
		errorCode = IFACE_ReleaseEntity(faceTemp[i]);
		error->CheckError(errorCode, error->medium);
		
	}

	/*errorCode = IFACE_ReleaseEntity(faceHandler);
	error->CheckError(errorCode, error->medium);*/
	delete[] faceTemp;

	return detectedFaces;
}

void FaceModel::ProcessFaceTracking(std::tuple<char*, unsigned char*, int*> dataSerialize, int client) {
	float rightEyeX, rightEyeY, leftEyeX, leftEyeY, faceConfidence;	
	int errorCode;		
	void* faceTrack = nullptr;
		
	errorCode = IFACE_CreateFace(&faceTrack);	
	
	errorCode = IFACE_DeserializeEntity(faceTrack, std::get<0>(dataSerialize),
		std::get<2>(dataSerialize)[3]);
	
	error->CheckError(errorCode, error->medium);
	
	if (errorCode == IFACE_OK) {
		errorCode = IFACE_GetFaceBasicInfo(faceTrack, faceHandlerGlobal,
			&rightEyeX, &rightEyeY, &leftEyeX, &leftEyeY, &faceConfidence);
		error->CheckError(errorCode, error->medium);
		//cout << "CONFIDENCE: " << faceConfidence << endl;
		if (faceConfidence > configuration->GetPrecision())
		{
			//cout << "GREATER OR EQUAL ACCURACY .." << configuration->GetPrecision() << endl;
			Molded* model = new Molded();
			model->SetMoldCropLength(std::get<2>(dataSerialize)[2]);
			model->SetCropImageData(std::get<1>(dataSerialize));
			model->SetMoldCropWidth(std::get<2>(dataSerialize)[0]);
			model->SetMoldCropHeight(std::get<2>(dataSerialize)[1]);
			//FaceCropImage(faceTrack, model);
			CreateTemplate(faceTrack, model, client);
			
		}
	}
	
	errorCode = IFACE_ReleaseEntity(faceTrack); 
	error->CheckError(errorCode, error->medium);
	/*delete[] std::get<0>(dataSerialize);
	delete[] std::get<1>(dataSerialize);
	get<2>(dataSerialize) = nullptr;*/
}

std::string FaceModel::IntToStr(int num)
{
	std::ostringstream s;
	s << num;
	return s.str();
}