#include "FaceModel.h"

FaceModel::FaceModel()
{
	file->SetNameDirectory("Logs");
	
	ObserverError();
}

FaceModel::~FaceModel()
{
	
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

void FaceModel::TerminateHandle() {
	int errorCode;
	errorCode = IFACE_ReleaseEntity(faceHandlerGlobal);
	error->CheckError(errorCode, error->medium);
}

// size image equals
int FaceModel::DetectByBatch(void* facesDetected[TOTAL_FACE_DETECTED], int client, int doing) {
	int errorCode, countFacesDetected = 0;
	const size_t numOfImages = bufferOfImagesBatch.size();
	const int maxFacesPerImage = configuration->GetMaxDetect();
	void*** faces = new void** [numOfImages];
	//int detectedFaces[11];
	int* detectedFaces = new int[numOfImages];
	//int* detectedFaces = (int*)calloc(numOfImages, sizeof(int));
	/*void** faces = new void* [maxFacesPerImage];
	for (int i = 0; i < maxFacesPerImage; i++)
	{
		errorCode = IFACE_CreateEntity(IFACE_ENTITY_TYPE_FACE, &(faces[i]));
		error->CheckError(errorCode, error->medium);
	}*/

	for (int i = 0; i < TOTAL_FACE_DETECTED; i++)
	{
		errorCode = IFACE_CreateEntity(IFACE_ENTITY_TYPE_FACE, &(facesDetected[i]));
		error->CheckError(errorCode, error->medium);
	}

	for (int i = 0; i < numOfImages; i++)
	{
		faces[i] = new void* [maxFacesPerImage];
		for (int j = 0; j < maxFacesPerImage; j++)
		{
			errorCode = IFACE_CreateEntity(IFACE_ENTITY_TYPE_FACE, &(faces[i][j]));
			error->CheckError(errorCode, error->medium);
		}
		detectedFaces[i] = maxFacesPerImage;
		
	}

	unsigned char** rawImagesData = new unsigned char*[numOfImages];

	for (int i = 0; i < numOfImages; i++)
	{
		vector<unsigned char> buffer = bufferOfImagesBatch[i];		
		rawImagesData[i] = &buffer[0];
	}

	cout << "Before IFACE_DetectFacesBatch: " << endl;
	errorCode = IFACE_DetectFacesBatch((int)numOfImages, rawImagesData, dimensionsImages[0][0],
		dimensionsImages[0][1], (float)configuration->GetMinFaceSize(), 
		(float)configuration->GetMaxFaceSize(), faceHandlerGlobal, detectedFaces, faces);
	error->CheckError(errorCode, error->medium);
	
	cout << "DETECT FACES: " << errorCode << endl;

	for (int imageIdx = 0; imageIdx < numOfImages; ++imageIdx)
	{
		cout << "INSIDE DETECT " << endl;
		if (detectedFaces[imageIdx] != 0) {
			for (int i = 0; i < detectedFaces[imageIdx]; i++)
			{   
				cout << "INSIDE DETECT FACES: " << detectedFaces[imageIdx] << endl;
				void* face = faces[imageIdx][i];
				cout << "OUT FACES " << imageIdx << " " << i << endl;
				if (countFacesDetected < TOTAL_FACE_DETECTED)
				{
					errorCode = IFACE_CloneEntity(face,
						facesDetected[countFacesDetected]);
					error->CheckError(errorCode, error->medium);
					cout << "ERROR FACES " << errorCode << endl;
					Molded* model = new Molded();
					model->SetIdClient(client);
					model->SetWhatDoing(doing);
					FaceCropImage(facesDetected[countFacesDetected], model);
					modelsDetected.push_back(model);
					countFacesDetected++;

				}
				
			}
		}

	}

	for (int i = 0; i < numOfImages; i++)
	{
		for (int j = 0; j < maxFacesPerImage; j++)
		{
			errorCode = IFACE_ReleaseEntity(faces[i][j]);
			error->CheckError(errorCode, error->medium);
		}
		delete[] faces[i];
		delete[] rawImagesData[i];
		
	}

	return countFacesDetected;
}

// size image different
int FaceModel::DetectByBatch(void* facesDetected[TOTAL_FACE_DETECTED],
	std::vector<std::vector<unsigned char>> bufferOfImagesBatch) {
	int errorCode, countFacesDetected = 0;
	int maxFaces = configuration->GetMaxDetect();
	string tracerSize, tracerConfidence;

	for (int i = 0; i < TOTAL_FACE_DETECTED; i++)
	{
		errorCode = IFACE_CreateEntity(IFACE_ENTITY_TYPE_FACE, &(facesDetected[i]));
		error->CheckError(errorCode, error->medium);
	}
	
	for (int i = 0; i < bufferOfImagesBatch.size() ; i++) 
	{
		int detectedFaces = configuration->GetMaxDetect();
		void** faceTemp = new void* [maxFaces];

		for (int j = 0; j < maxFaces; j++) {
			errorCode = IFACE_CreateEntity(IFACE_ENTITY_TYPE_FACE, &(faceTemp[j]));
			error->CheckError(errorCode, error->medium);
		}
		
		vector<unsigned char> buffer = bufferOfImagesBatch[i];
				
		errorCode = IFACE_DetectFaces(&buffer[0],
			dimensionsImages[i][0], dimensionsImages[i][1],
			(float)configuration->GetMinFaceSize(),
			(float)configuration->GetMaxFaceSize(),
			faceHandlerGlobal, &detectedFaces, faceTemp);		
		error->CheckError(errorCode, error->medium);
		
		buffer.clear();
				
		if (detectedFaces != EMPTY_FACE)
		{
			for (int j = 0; j < detectedFaces; j++) {
				
				void* face = faceTemp[j];
				
				float rightEyeX, rightEyeY, leftEyeX, leftEyeY;
				float faceConfidence;
				
				errorCode = IFACE_GetFaceBasicInfo(face, faceHandlerGlobal,
					&rightEyeX, &rightEyeY, &leftEyeX, &leftEyeY, &faceConfidence);
				error->CheckError(errorCode, error->medium);
				//tracerImage += "(" + to_string(dimensionsImages[i][0]) + "-" + to_string(dimensionsImages[i][1]) + "), ";
				//tracerImage += to_string(faceConfidence) + " vs " + to_string(configuration->GetConfidenceThreshold()) + "\n";
				
				tracerSize += "(" + to_string(dimensionsImages[i][0]) + "-" + to_string(dimensionsImages[i][1]) + ") ";
				tracerConfidence += to_string(faceConfidence) + " ";
				if (countFacesDetected < TOTAL_FACE_DETECTED)
				{					
					errorCode = IFACE_CloneEntity(face,
						facesDetected[countFacesDetected]);
					error->CheckError(errorCode, error->medium);

					/*Molded* model = new Molded();
					model->SetIdClient(client);
					model->SetWhatDoing(doing);
					FaceCropImage(facesDetected[countFacesDetected], model);
					modelsDetected.push_back(model);*/
					countFacesDetected++;

				}
				
			}

		}
		else {

			/*tracerImage += "To refuse: (" + to_string(dimensionsImages[i][0]) +
				"-" + to_string(dimensionsImages[i][1]) + "), CONFIDENCE THRESHOLD: " + 
				to_string(configuration->GetConfidenceThreshold()) + "\n ";		*/
			tracerSize += "(" + to_string(dimensionsImages[i][0]) + "-" + to_string(dimensionsImages[i][1]) + ") ";
			tracerConfidence += "(To_refuse) ";
			
		}

		for (int j = 0; j < maxFaces; j++) {
			errorCode = IFACE_ReleaseEntity(faceTemp[j]);
			error->CheckError(errorCode, error->medium);
		}

	}	
	tracerProcess.push_back(tracerSize);
	tracerProcess.push_back(tracerConfidence);
	tracerProcess.push_back(to_string(configuration->GetConfidenceThreshold()));
	return countFacesDetected;
}


void FaceModel::FaceCropImage(void* face, Molded* model) {
	int cropWidth = -1, cropHeight = -1, cropLength = -1, errorCode = -1;

	switch (configuration->GetGetFaceCropImage())
	{
	case 0:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal,
			IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, NULL);
		error->CheckError(errorCode, error->medium);
		break;
	case 1:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal,
			IFACE_FACE_CROP_METHOD_FULL_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, NULL);
		error->CheckError(errorCode, error->medium);
		break;
	case 2:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal,
			IFACE_FACE_CROP_METHOD_FULL_FRONTAL_EXTENDED,
			&cropWidth, &cropHeight, &cropLength, NULL);
		error->CheckError(errorCode, error->medium);
		break;
	case 3:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal,
			IFACE_FACE_CROP_METHOD_TOKEN_NOT_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, NULL);
		error->CheckError(errorCode, error->medium);
		break;
	case 4:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal,
			IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL_EXTENDED,
			&cropWidth, &cropHeight, &cropLength, NULL);
		error->CheckError(errorCode, error->medium);
		break;
	default:
		break;
	}	
	unsigned char* cropImageData = new unsigned char[cropLength];
	
	switch (configuration->GetGetFaceCropImage())
	{
	case 0:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal,
			IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, cropImageData);
		error->CheckError(errorCode, error->medium);
		break;
	case 1:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal,
			IFACE_FACE_CROP_METHOD_FULL_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, cropImageData);
		error->CheckError(errorCode, error->medium);
		break;
	case 2:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal,
			IFACE_FACE_CROP_METHOD_FULL_FRONTAL_EXTENDED,
			&cropWidth, &cropHeight, &cropLength, cropImageData);
		error->CheckError(errorCode, error->medium);
		break;
	case 3:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal,
			IFACE_FACE_CROP_METHOD_TOKEN_NOT_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, cropImageData);
		error->CheckError(errorCode, error->medium);
		break;
	case 4:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerGlobal,
			IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL_EXTENDED,
			&cropWidth, &cropHeight, &cropLength, cropImageData);
		error->CheckError(errorCode, error->medium);
		break;
	default:
		break;
	}	

	if (errorCode == IFACE_OK) {
		model->SetMoldCropLength(cropLength);
		model->SetCropImageData(cropImageData);
		model->SetMoldCropWidth(cropWidth);
		model->SetMoldCropHeight(cropHeight);
		
	}
	
	delete[] cropImageData;
}

void FaceModel::CropDetectedFaces(int countFacesDetected, void* facesDetected[TOTAL_FACE_DETECTED],
	int client, int doing) {
	
	for (int i = 0; i < countFacesDetected; i++)
	{
		Molded* model = new Molded();
		model->SetIdClient(client);
		model->SetWhatDoing(doing);
		FaceCropImage(facesDetected[i], model);
		modelsDetected.push_back(model);
		
	}
}

int FaceModel::ModelByBatch(int client,  int doing) {
	int errorCode, facesValid = 0;
	void* facesDetected[TOTAL_FACE_DETECTED];

	int countFacesDetected = DetectByBatch(facesDetected, bufferOfImagesBatch);
	//int countFacesDetected = DetectByBatch(facesDetected, client, doing);
	//tracerImage += "Face detected: " + to_string(countFacesDetected) + "\n";
	tracerProcess.push_back(to_string(countFacesDetected));
	if (countFacesDetected != 0)
	{
		CropDetectedFaces(countFacesDetected, facesDetected, client, doing);
		facesValid = GetBatchModels(countFacesDetected, facesDetected);
	}

	for (int i = 0; i < TOTAL_FACE_DETECTED; i++)
	{

		errorCode = IFACE_ReleaseEntity(facesDetected[i]);
		error->CheckError(errorCode, error->medium);
	}
	bufferOfImagesBatch.clear();
	dimensionsImages.clear();
	return facesValid;
}

int FaceModel::GetBatchModels(int countFacesDetected, 
	void* facesDetected[TOTAL_FACE_DETECTED]) {
	int errorCode, majorVersion, minorVersion, quality, count, templateBatchDataSize;	
	void** cloneFacesDetected = new void*[countFacesDetected];
	char** batchTemplates = new char*[countFacesDetected];
	string tracerQuality = "";

	for (int i = 0; i < countFacesDetected; i++)
	{
		errorCode = IFACE_CreateEntity(IFACE_ENTITY_TYPE_FACE, &(cloneFacesDetected[i]));		
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

	
	std::vector<vector<int>> OrderQualityTemplate;

	for (int i = 0; i < countFacesDetected; i++) {

		errorCode = IFACE_GetTemplateInfo(faceHandlerGlobal,
			batchTemplates[i], &majorVersion, &minorVersion, &quality);
		//cout << "QUALITY TEMPLATE: " << quality << " QUALITY TEMPLATE CONFIGURATION: " << configuration->GetQualityModel() << endl;
		//tracerImage += to_string(quality) + " vs " + to_string(configuration->GetQualityModel()) + "\n";
		tracerQuality += to_string(quality) + " ";
		if (quality > configuration->GetQualityModel()) {
			std::vector<int> orderQuality = { quality, i};
			OrderQualityTemplate.push_back(orderQuality);
		}
		
	}

	tracerProcess.push_back(tracerQuality);
	tracerProcess.push_back(to_string(configuration->GetQualityModel()));

	if (!OrderQualityTemplate.empty()) {
		std::sort(OrderQualityTemplate.rbegin(), OrderQualityTemplate.rend());
		count = (int)OrderQualityTemplate.size();
		//tracerImage += "Accepted models: " + to_string(count) + "\n";
		tracerProcess.push_back(to_string(count));
		tracerImage = BuildTracerString();
		for (int i = 0; i < OrderQualityTemplate.size(); i++)
		{
			//cout << "ORDER QUALITY TEMPLATES: " << OrderQualityTemplate[i][0] << " " << OrderQualityTemplate[i][1] << endl;
			int j = OrderQualityTemplate[i][1];
			int qualityUp = OrderQualityTemplate[i][0];
			int sizeImage[6];
			sizeImage[0] = modelsDetected[j]->GetMoldCropWidth();
			sizeImage[1] = modelsDetected[j]->GetMoldCropHeight();
			sizeImage[2] = templateBatchDataSize;
			sizeImage[3] = qualityUp;
			sizeImage[4] = modelsDetected[j]->GetWhatDoing();
			sizeImage[5] = (int)OrderQualityTemplate.size();

			auto tupleTemplateFace = std::make_tuple(batchTemplates[j],
				modelsDetected[j]->GetCropImageData(), sizeImage, tracerImage);
			templateOut.on_next(tupleTemplateFace);

		}
	}
	else {
		count = 0;
		tracerProcess.push_back(to_string(count));
		tracerImage = BuildTracerString() + "\n";
		file->WriteFile(tracerImage);
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
	OrderQualityTemplate.clear();
	modelsDetected.clear();

	return count;
}

unsigned char* FaceModel::LoadImageMemory(vector<unsigned char> buffer, 
	int *width, int *height, int *lenght) {
	int errorCode;
	const char* imgData = reinterpret_cast<const char*> (&buffer[0]);
	if (imgData == NULL) {
		return NULL;
	}

	errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
		height, lenght, NULL);
	if (errorCode != IFACE_OK) {
		error->CheckError(errorCode, error->medium);
		return NULL;
	}
	unsigned char* rawImage = new unsigned char[*lenght];
	errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
		height, lenght, rawImage);
	if (errorCode != IFACE_OK) {
		error->CheckError(errorCode, error->medium);
		return NULL;
	}

	return rawImage;

}

vector<string> FaceModel::LoadFilesForBatch(string folder, string& filesFolder) {
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
		filesFolder += iter->path().string() + " ";
	}	
	
	return listPathFile;
	
}

void FaceModel::LoadImagesForBatch(vector<string> listFiles) {
	int lenght, width, height, count = 0;
	if (!listFiles.empty())
	{
		for (int i = 0; i < listFiles.size(); i++)
		{				
			unsigned char* rawImage;

			if (file->GetImageSizeEx(listFiles[i].c_str(), &width, &height)) {

				if (width <= MAX_SIZE_IMAGE) {
					rawImage = LoadFileImage(listFiles[i], &width, &height, &lenght);
				}
				else
				{
					vector<uchar> newImg = file->ResizeImage(listFiles[i].c_str(), width, height);
					rawImage = LoadImageMemory(newImg, &width, &height, &lenght);
				}

				if (rawImage != NULL && count < BATCH_TOTAL_SIZE) {

					vector<unsigned char> bufferImage;
					bufferImage.assign(rawImage, rawImage + lenght);
					bufferOfImagesBatch.push_back(bufferImage);
					std::vector<int> sizeImage = { width, height, lenght };
					dimensionsImages.push_back(sizeImage);
					count++;
					delete[] rawImage;
				}
				
			}
								
		}		
	}			
}

void FaceModel::AddCollectionOfImages(string folder, int client, int doing) {
	string filesInFolder = " ";
	tracerProcess.clear();
	if (file->GetNameFile().length() == 0)
	{
		string nameFileTracer = file->GetNameLog() + to_string(client) + ".cvs";
		file->SetNameFile(nameFileTracer);
		
	}
	char buff[20];
	
	time_t now = time(NULL);
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
	string timeInit(buff);
		
	tracerProcess.push_back(timeInit);
	tracerProcess.push_back(to_string(client));	
	tracerProcess.push_back(to_string(2));
	vector<string> listFiles = LoadFilesForBatch(folder, filesInFolder);
	tracerProcess.push_back(filesInFolder);
	LoadImagesForBatch(listFiles);
	if (!bufferOfImagesBatch.empty())
	{		
		
		tracerProcess.push_back(to_string((int)bufferOfImagesBatch.size()));
		int detectNumber = ModelByBatch(client, doing);
		
	}
	else {
		
		tracerProcess.push_back("0");
		tracerImage = BuildTracerString() + "\n";
		file->WriteFile(tracerImage);
	}
}

void FaceModel::RecognitionFaceFiles(string namefile, int client, int task) {	
	int lenght, width, height, templates;
	char buff[20];

	//std::cout << namefile << std::endl;

	tracerProcess.clear();
	if (file->GetNameFile().length() == 0)
	{
		string nameFileTracer = file->GetNameLog() + to_string(client) + ".cvs";
		file->SetNameFile(nameFileTracer);

	}

	time_t now = time(NULL);
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));

	string timeInit(buff);
	tracerProcess.push_back(timeInit);
	tracerProcess.push_back(to_string(client));
	tracerProcess.push_back(to_string(2));
	tracerProcess.push_back(namefile);

	if (file->GetImageSizeEx(namefile.c_str(), &width, &height))
	{

		unsigned char* rawImage;

		if (width <= MAX_SIZE_IMAGE)
		{
			rawImage = LoadFileImage(namefile, &width, &height, &lenght);

		}
		else
		{
			vector<uchar> newImg = file->ResizeImage(namefile.c_str(), width, height);
			rawImage = LoadImageMemory(newImg, &width, &height, &lenght);

		}
		if (rawImage != NULL) {
			tracerProcess.push_back("1");

			templates = GetOneModel(rawImage, width, height, client, task);
			delete[] rawImage;
		}

	}
	else
	{
		string errorContent = timeInit + " Error file image format (" + namefile + ")\n";
		string pathError = "Logs/errors.txt";
		file->WriteFile(pathError, errorContent);
		isFinishLoadFiles = true;
	}

	//this_thread::sleep_for(chrono::milliseconds(20));
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

void FaceModel::CreateTemplate(void* face, Molded* model, int client, int task) {
	int errorCode, majorVersion, minorVersion, quality;
	int templateSize;
	string logTemplate = "", tracerImageForSend = "";
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
			//cout << "QUALITY MODEL: " << quality << endl;
			//cout << "CONFIGURATION MODEL: " << configuration->GetQualityModel() << endl;
			logTemplate = to_string(quality) + ", " + to_string(configuration->GetQualityModel());

			//tracerProcess.push_back(to_string(quality));
			//tracerProcess.push_back(to_string(configuration->GetQualityModel()));

			//tracerImage +=  to_string(quality) + " vs " + to_string(configuration->GetQualityModel()) + ", ";

			if (quality > configuration->GetQualityModel()) {
				logTemplate += ", 1, ";
				//tracerProcess.push_back("1");
				//tracerImage += "1, ";
				
				tracerImageForSend = BuildTracerString() + logTemplate;
				
				int sizeImage[6];
				sizeImage[0] = model->GetMoldCropWidth();
				sizeImage[1] = model->GetMoldCropHeight();
				sizeImage[2] = templateSize;
				sizeImage[3] = quality;
				sizeImage[4] = task; // doing
				sizeImage[5] = 0; // priority of template
				//cout << "TRACER FILE.........:" << tracerImage << endl;
				
				auto tupleTemplateFace = std::make_tuple(templateData,
					model->GetCropImageData(), sizeImage, tracerImageForSend);
				
				templateOut.on_next(tupleTemplateFace);
				//cout << "TRACER FILE.........:" << tracerImageForSend << endl;
			}
			else {
				//tracerProcess.push_back("0");
				logTemplate += ", 0, ";
				tracerImageForSend = BuildTracerString() + logTemplate + "\n";
				file->WriteFile(tracerImageForSend);
				isFinishLoadFiles = true;
			}
			
		}
		delete[] templateData;
	}
	else {
		error->CheckError(errorCode, error->medium);
		isFinishLoadFiles = true;
	}	
}

string FaceModel::BuildTracerString() {
	string result = "";
	for (string value : tracerProcess) {
		result += value + ", ";
	}
	if ((int)tracerProcess.size() > 4)
	{
		tracerProcess.pop_back();
		tracerProcess.pop_back();
		tracerProcess.pop_back();
		tracerProcess.pop_back();
	}
	
	//tracerProcess.clear();
	return result;
}

void FaceModel::InitHandle() {
	int errorCode;

	errorCode = IFACE_CreateFaceHandler(&faceHandlerGlobal);
	error->CheckError(errorCode, error->medium);

	if (configuration->GetFaceTemplextspeedAccurancyMode() == 0) {
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_DEFAULT);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetFaceTemplextspeedAccurancyMode() == 1) {
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_BALANCED);
		error->CheckError(errorCode, error->medium);
	}

	if (configuration->GetFaceTemplextspeedAccurancyMode() == 2) {
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_ACCURATE);
		error->CheckError(errorCode, error->medium);
	}

	if (configuration->GetFaceTemplextspeedAccurancyMode() == 3) {
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_FAST);
		error->CheckError(errorCode, error->medium);
	}


	if (configuration->GetSpeedAccurancyMode() == 0)
	{
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_DEFAULT);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetSpeedAccurancyMode() == 1)
	{
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_BALANCED);
		error->CheckError(errorCode, error->medium);
	}

	if (configuration->GetSpeedAccurancyMode() == 2)
	{
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_ACCURATE);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetSpeedAccurancyMode() == 3)
	{
		errorCode = IFACE_SetParam(faceHandlerGlobal,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_FAST);
		error->CheckError(errorCode, error->medium);
	}

	errorCode = IFACE_SetParam(faceHandlerGlobal,
		IFACE_PARAMETER_FACEDET_CONFIDENCE_THRESHOLD,
		IntToStr(configuration->GetConfidenceThreshold()).c_str());
	error->CheckError(errorCode, error->medium);
	
	//errorCode = IFACE_SetParam(faceHandlerGlobal,
	//	IFACE_PARAMETER_TRACK_MAX_FACE_SIZE,
	//	IntToStr(configuration->GetMaxFaceSize()).c_str()); //IFACE_TRACK_MAX_FACE_SIZE_DEFAULT 400
	//error->CheckError(errorCode, error->medium);
	//
	//errorCode = IFACE_SetParam(faceHandlerGlobal,
	//	IFACE_PARAMETER_TRACK_MIN_FACE_SIZE,
	//	IntToStr(configuration->GetMinFaceSize()).c_str()); //IFACE_TRACK_MIN_FACE_SIZE_DEFAULT 18
	//error->CheckError(errorCode, error->medium);
	
	
}

int FaceModel::GetOneModel(unsigned char* rawImage,
	int width, int height, int client, int task) {
	int maxFaces = configuration->GetMaxDetect();
	int detectedFaces = configuration->GetMaxDetect();
	int errorCode;
	void** faceTemp = new void*[maxFaces];

	for (int i = 0; i < maxFaces; i++) {
		errorCode = IFACE_CreateEntity(IFACE_ENTITY_TYPE_FACE, &(faceTemp[i]));
		error->CheckError(errorCode, error->medium);
	}

	errorCode = IFACE_DetectFaces(rawImage, width, height,
		(float)configuration->GetMinFaceSize(), (float)configuration->GetMaxFaceSize(),
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
				//cout << "CONFIDENCE: " << faceConfidence << endl;
				tracerProcess.push_back("(" + to_string(width) + "-" + to_string(height) + ") ");
				
				tracerProcess.push_back(to_string(faceConfidence));
				tracerProcess.push_back(to_string(configuration->GetConfidenceThreshold()));
				if (faceConfidence > configuration->GetConfidenceThreshold())
				{
					
					tracerProcess.push_back("1");
					//cout << "GREATER OR EQUAL ACCURACY .." << i << endl;
					Molded* model = new Molded();
					
					FaceCropImage(face, model);
					
					CreateTemplate(face, model, client, task);
					delete model;
					
				}
				else
				{
					tracerProcess.push_back("0");
					tracerImage = BuildTracerString() + "\n";
					file->WriteFile(tracerImage);
					isFinishLoadFiles = true;
				}
				

			}
		}
		else {											
			tracerProcess.push_back("(" + to_string(width) + "-" + to_string(height) + ")");
			tracerProcess.push_back("To refuse");
			tracerProcess.push_back(to_string(configuration->GetConfidenceThreshold()));
			tracerProcess.push_back("0");
			tracerImage = BuildTracerString() + "\n";
			file->WriteFile(tracerImage);
			isFinishLoadFiles = true;
		}
	}
	else {
		tracerProcess.push_back("(" + to_string(width) + "-" + to_string(height) + ")");
		tracerProcess.push_back("To refuse");
		tracerProcess.push_back(to_string(configuration->GetConfidenceThreshold()));
		tracerProcess.push_back("0");
		tracerImage = BuildTracerString() + "\n";
		file->WriteFile(tracerImage);
		error->CheckError(errorCode, error->medium);
		isFinishLoadFiles = true;
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