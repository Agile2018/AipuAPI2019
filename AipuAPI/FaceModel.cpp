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
	int client, int doing) {
	int errorCode, countFacesDetected = 0;
	int maxFaces = configuration->GetMaxDetect();

	for (int i = 0; i < BATCH_TOTAL_SIZE; i++)
	{
		errorCode = IFACE_CreateFace(&(facesDetected[i]));
		error->CheckError(errorCode, error->medium);
	}
	
	for (int i = 0; i < bufferOfImagesBatch.size() ; i++) 
	{
		int detectedFaces = configuration->GetMaxDetect();
		void** faceTemp = new void* [maxFaces];

		for (int j = 0; j < maxFaces; j++) {
			errorCode = IFACE_CreateFace(&(faceTemp[j]));
			error->CheckError(errorCode, error->medium);
		}

		vector<unsigned char> buffer = bufferOfImagesBatch[i];
				
		errorCode = IFACE_DetectFaces(&buffer[0],
			dimensionsImages[i][0], dimensionsImages[i][1],
			(float)configuration->GetMinEyeDistance(),
			(float)configuration->GetMaxEyeDistance(),
			faceHandlerGlobal, &detectedFaces, faceTemp);		
		error->CheckError(errorCode, error->medium);
		
		buffer.clear();
				
		if (detectedFaces != EMPTY_FACE)
		{
			for (int j = 0; j < detectedFaces; j++) {
				/*float rightEyeX, rightEyeY, leftEyeX, leftEyeY;
				float faceConfidence;*/
				void* face = faceTemp[j];
				
				/*errorCode = IFACE_GetFaceBasicInfo(face, faceHandlerGlobal,
					&rightEyeX, &rightEyeY, &leftEyeX, &leftEyeY, &faceConfidence);
				error->CheckError(errorCode, error->medium);
				cout << "CONFIDENCE: " << faceConfidence << endl;*/
				if (countFacesDetected < BATCH_TOTAL_SIZE)
				{					
					errorCode = IFACE_CloneEntity(face,
						facesDetected[countFacesDetected]);
					error->CheckError(errorCode, error->medium);

					Molded* model = new Molded();
					model->SetIdClient(client);
					model->SetWhatDoing(doing);
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
	
	delete[] cropImageData;
}

int FaceModel::ModelByBatch(int client,  int doing) {
	int errorCode, facesValid = 0;
	void* facesDetected[BATCH_TOTAL_SIZE];
	int countFacesDetected = DetectByBatch(facesDetected, bufferOfImagesBatch, client, doing);
	if (countFacesDetected != 0)
	{
		facesValid = GetBatchModels(countFacesDetected, facesDetected);
	}

	for (int i = 0; i < BATCH_TOTAL_SIZE; i++)
	{
		errorCode = IFACE_ReleaseEntity(facesDetected[i]);
		error->CheckError(errorCode, error->medium);
	}
	bufferOfImagesBatch.clear();
	dimensionsImages.clear();
	return facesValid;
}

int FaceModel::GetBatchModels(int countFacesDetected, 
	void* facesDetected[BATCH_TOTAL_SIZE]) {
	int errorCode, majorVersion, minorVersion, quality;;
	int templateBatchDataSize;
	void** cloneFacesDetected = new void*[countFacesDetected];
	char** batchTemplates = new char*[countFacesDetected];
	

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

	errorCode = IFACE_CreateTemplateBatch(faceHandlerGlobal, 
		countFacesDetected, cloneFacesDetected,
		&templateBatchDataSize, NULL);
	error->CheckError(errorCode, error->medium);

	for (int i = 0; i < countFacesDetected; ++i)
	{

		batchTemplates[i] = new char[templateBatchDataSize];
	}

	errorCode = IFACE_CreateTemplateBatch(faceHandlerGlobal, 
		countFacesDetected, cloneFacesDetected,
		&templateBatchDataSize, batchTemplates);
	error->CheckError(errorCode, error->medium);

	int count = 0;

	for (int i = 0; i < countFacesDetected; i++)
	{		
		errorCode = IFACE_GetTemplateInfo(faceHandlerGlobal,
			batchTemplates[i], &majorVersion, &minorVersion, &quality);
		if (quality > configuration->GetQualityModel()) {
			int sizeImage[6];
			sizeImage[0] = modelsDetected[i]->GetMoldCropWidth();
			sizeImage[1] = modelsDetected[i]->GetMoldCropHeight();
			sizeImage[2] = templateBatchDataSize;
			sizeImage[3] = quality;
			sizeImage[4] = modelsDetected[i]->GetWhatDoing();
			sizeImage[5] = count;
			
			auto tupleTemplateFace = std::make_tuple(batchTemplates[i],
				modelsDetected[i]->GetCropImageData(), sizeImage);
			templateOut.on_next(tupleTemplateFace);
			
			if (count == 0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
			count++;
		}		

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
	
	modelsDetected.clear();

	return count;
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

vector<string> FaceModel::LoadFilesForBatch(string folder) {	
	vector<string> listPathFile;
	std::set<std::string> targetExtensions;
	targetExtensions.insert(".JPG");
	targetExtensions.insert(".BMP");	
	targetExtensions.insert(".PNG");	
	path folderPath(folder);
	
	fs::directory_iterator end;
	for (fs::directory_iterator iter(folderPath); iter != end; ++iter) {
		if (!fs::is_regular_file(iter->status())) { continue; }

		std::string extension = iter->path().extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::toupper);
		if (targetExtensions.find(extension) == targetExtensions.end()) { continue; }		
		listPathFile.push_back(iter->path().string());
		
	}	
	
	return listPathFile;
	
}

void FaceModel::LoadImagesForBatch(vector<string> listFiles) {
	int lenght, width, height, count = 0;
	if (!listFiles.empty())
	{
		for (int i = 0; i < listFiles.size(); i++)
		{					
			unsigned char* rawImage = LoadFileImage(listFiles[i],
				&width, &height, &lenght);

			if (rawImage != NULL && count < BATCH_TOTAL_SIZE) {
				vector<unsigned char> bufferImage;
				bufferImage.assign(rawImage, rawImage + lenght);
				bufferOfImagesBatch.push_back(bufferImage);
				std::vector<int> sizeImage = { width, height, lenght};				
				dimensionsImages.push_back(sizeImage);
				count++;
			}
			delete[] rawImage;			

		}		
	}			
}

void FaceModel::AddCollectionOfImages(string folder, int client, int doing) {
	vector<string> listFiles = LoadFilesForBatch(folder);
	LoadImagesForBatch(listFiles);
	if (!bufferOfImagesBatch.empty())
	{		
		int detectNumber = ModelByBatch(client, doing);
		cout << "FACES DETECTED AND VALIDATED: " << detectNumber << endl;
	}
			
}

void FaceModel::RecognitionFaceFiles(string file, int client) {
	int lenght, width, height, templates;
	
	isFinishLoadFiles = false;	
	cout << "FILE OF THE IMAGE: " << file << endl;
	unsigned char* rawImage = LoadFileImage(file, &width, &height, &lenght);
	if (rawImage != NULL) {
		templates = GetOneModel(rawImage, width, height, client);
	}
	delete[] rawImage;
	
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
			cout << "QUALITY MODEL: " << quality << endl;
			cout << "CONFIGURATION MODEL: " << configuration->GetQualityModel() << endl;
			if (quality > configuration->GetQualityModel()) {
				
				int sizeImage[6];
				sizeImage[0] = model->GetMoldCropWidth();
				sizeImage[1] = model->GetMoldCropHeight();
				sizeImage[2] = templateSize;
				sizeImage[3] = quality;
				sizeImage[4] = 0; // doing
				sizeImage[5] = 0; // priority of template

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

	errorCode = IFACE_SetParam(faceHandlerGlobal,
		IFACE_PARAMETER_FACEDET_CONFIDENCE_THRESHOLD,
		IntToStr(configuration->GetPrecision()).c_str());
	error->CheckError(errorCode, error->medium);
	

}

int FaceModel::GetOneModel(unsigned char* rawImage,
	int width, int height, int client) {
	int maxFaces = configuration->GetMaxDetect();
	int detectedFaces = configuration->GetMaxDetect();
	int errorCode;
	void** faceTemp = new void*[maxFaces];
	
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

	delete[] faceTemp;

	return detectedFaces;
}


std::string FaceModel::IntToStr(int num)
{
	std::ostringstream s;
	s << num;
	return s.str();
}