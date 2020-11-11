#include "Tracking.h"


Tracking::Tracking()
{
	file->SetNameDirectory("Logs");
	ObserverError();
}

Tracking::~Tracking()
{
	
}

void Tracking::InitITracking() {
	int errorCode;

	errorCode = IFACE_CreateFaceHandler(&faceHandlerTracking);
	error->CheckError(errorCode, error->medium);

	if (configurationIFaceProcessing->GetFaceTemplextspeedAccurancyMode() == 0) {
		errorCode = IFACE_SetParam(faceHandlerTracking,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_DEFAULT);
		error->CheckError(errorCode, error->medium);
	}
	if (configurationIFaceProcessing->GetFaceTemplextspeedAccurancyMode() == 1) {
		errorCode = IFACE_SetParam(faceHandlerTracking,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_BALANCED);
		error->CheckError(errorCode, error->medium);
	}

	if (configurationIFaceProcessing->GetFaceTemplextspeedAccurancyMode() == 2) {
		errorCode = IFACE_SetParam(faceHandlerTracking,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_ACCURATE);
		error->CheckError(errorCode, error->medium);
	}

	if (configurationIFaceProcessing->GetFaceTemplextspeedAccurancyMode() == 3) {
		errorCode = IFACE_SetParam(faceHandlerTracking,
			IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
			IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_FAST);
		error->CheckError(errorCode, error->medium);
	}


	if (configurationIFaceProcessing->GetSpeedAccurancyMode() == 0)
	{
		errorCode = IFACE_SetParam(faceHandlerTracking,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_DEFAULT);
		error->CheckError(errorCode, error->medium);
	}
	if (configurationIFaceProcessing->GetSpeedAccurancyMode() == 1)
	{
		errorCode = IFACE_SetParam(faceHandlerTracking,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_BALANCED);
		error->CheckError(errorCode, error->medium);
	}

	if (configurationIFaceProcessing->GetSpeedAccurancyMode() == 2)
	{
		errorCode = IFACE_SetParam(faceHandlerTracking,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_ACCURATE);
		error->CheckError(errorCode, error->medium);
	}
	if (configurationIFaceProcessing->GetSpeedAccurancyMode() == 3)
	{
		errorCode = IFACE_SetParam(faceHandlerTracking,
			IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
			IFACE_FACEDET_SPEED_ACCURACY_MODE_FAST);
		error->CheckError(errorCode, error->medium);
	}

	errorCode = IFACE_SetParam(faceHandlerTracking,
		IFACE_PARAMETER_FACEDET_CONFIDENCE_THRESHOLD,
		IntToStr(configurationIFaceProcessing->GetConfidenceThreshold()).c_str());
	error->CheckError(errorCode, error->medium);

	/*errorCode = IFACE_SetParam(faceHandlerTracking,
		IFACE_PARAMETER_TRACK_MAX_FACE_SIZE,
		IFACE_TRACK_MAX_FACE_SIZE_DEFAULT);
	error->CheckError(errorCode, error->medium);

	errorCode = IFACE_SetParam(faceHandlerTracking,
		IFACE_PARAMETER_TRACK_MIN_FACE_SIZE,
		IFACE_TRACK_MIN_FACE_SIZE_DEFAULT);
	error->CheckError(errorCode, error->medium);*/

	errorCode = IFACE_CreateObjectHandler(&objectHandler, faceHandlerTracking);
	error->CheckError(errorCode, error->medium);

	for (int i = 0; i < NUM_TRACKED_OBJECTS; i++)
	{
		errorCode = IFACE_CreateObject(&objects[i]);
		error->CheckError(errorCode, error->medium);
	}
	

	errorCode = IFACE_SetParam(objectHandler,
		IFACE_PARAMETER_TRACK_FACE_DISCOVERY_FREQUENCE_MS,
		IntToStr(configuration->GetDiscoveryFrequenceMS()).c_str());
	error->CheckError(errorCode, error->medium);
		
	if (configuration->GetTrackingMode() == 0)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_TRACKING_MODE,
			IFACE_TRACK_TRACKING_MODE_DEFAULT);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetTrackingMode() == 1)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_TRACKING_MODE,
			IFACE_TRACK_TRACKING_MODE_LIVENESS_DOT);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetTrackingMode() == 2)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_TRACKING_MODE,
			IFACE_TRACK_TRACKING_MODE_OBJECT_TRACKING);
		error->CheckError(errorCode, error->medium);
	}

	if (configuration->GetTrackSpeed() == 0)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_SPEED_ACCURACY_MODE,
			IFACE_TRACK_SPEED_ACCURACY_MODE_ACCURATE);
		error->CheckError(errorCode, error->medium);
	}

	if (configuration->GetTrackSpeed() == 1)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_SPEED_ACCURACY_MODE,
			IFACE_TRACK_SPEED_ACCURACY_MODE_BALANCED);
		error->CheckError(errorCode, error->medium);
	}

	if (configuration->GetTrackSpeed() == 2)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_SPEED_ACCURACY_MODE,
			IFACE_TRACK_SPEED_ACCURACY_MODE_FAST);
		error->CheckError(errorCode, error->medium);
	}

	if (configuration->GetMotionOptimization() == 0)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
			IFACE_TRACK_MOTION_OPTIMIZATION_DISABLED);
		error->CheckError(errorCode, error->medium);
	}

	if (configuration->GetMotionOptimization() == 1)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
			IFACE_TRACK_MOTION_OPTIMIZATION_HISTORY_LONG_ACCURATE);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetMotionOptimization() == 2)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
			IFACE_TRACK_MOTION_OPTIMIZATION_HISTORY_LONG_FAST);
		error->CheckError(errorCode, error->medium);
	}
	if (configuration->GetMotionOptimization() == 3)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
			IFACE_TRACK_MOTION_OPTIMIZATION_HISTORY_SHORT);
		error->CheckError(errorCode, error->medium);
	}

	errorCode = IFACE_SetParam(objectHandler,
		IFACE_PARAMETER_TRACK_DEEP_TRACK,
		configuration->GetDeepTrack().c_str()); 
	error->CheckError(errorCode, error->medium);	

	flagTracking = false;
	
}

void Tracking::FaceTracking(std::vector<unsigned char> vectorData) {
	int width, height, errorCode;
	
	unsigned char* rawImageData = LoadImageOfMemory(vectorData, &width, &height);
	

	if (rawImageData != NULL) {
		long secuence = countFrameTracking * configuration->GetTimeDeltaMs();
		ResetCoordinates();
		
		errorCode = IFACE_TrackObjects(objectHandler, rawImageData,
			width, height, secuence, NUM_TRACKED_OBJECTS, objects);			
		error->CheckError(errorCode, error->medium);
		if (errorCode != IFACE_OK) {
			cout << "..........ERRRRORORRRRRRR IFACE_TrackObjects.............................." << errorCode << endl;
		}
		
		TrackObjectState();

		delete[] rawImageData;
	}
	flagTracking = false;
	vectorData.clear();
}


void Tracking::BuildCoordinatesImage(float x, float y, float width, 
	float height, int indexTracked) {
	int index = indexTracked * NUM_COORDINATES_X_IMAGE;
	imageCoordinatesFollowed[index] = x;
	imageCoordinatesFollowed[index + 1] = y;
	imageCoordinatesFollowed[index + 2] = width;
	imageCoordinatesFollowed[index + 3] = height;
}

void Tracking::SetColorRectangle(float score, int indexObject) {
	float percentageScore = ((float)score * 100.0f) / 10000.0f;
	float variationColor = (1.0f * percentageScore) / 100.0f;
	float color = (1.0f - variationColor);
	colorRectangle[indexObject] = color;
}

void Tracking::ClearCoordinatesImage(int indexTracked) {
	int index = indexTracked * NUM_COORDINATES_X_IMAGE;
	imageCoordinatesFollowed[index] = 0;
	imageCoordinatesFollowed[index + 1] = 0;
	imageCoordinatesFollowed[index + 2] = 0;
	imageCoordinatesFollowed[index + 3] = 0;
}

void Tracking::ClearAllCoordinatesImage() {
	for (int i = 0; i < NUM_TRACKED_OBJECTS; i++) {
		ClearCoordinatesImage(i);
	}
}

void Tracking::ResetCoordinates() {
	long countFrames = countFrameTracking - 1;
	int residue = countFrames % sizeVideoStream;
	if (residue == 0)
	{
		ClearAllCoordinatesImage();
	}
}

void Tracking::RunTask(void* face, Molded* model) {
	switch (taskIdentify)
	{
	case -1:
		cout << "TASK: " << taskIdentify << endl;
		break;
	case 0: //Import user
		FaceCropImage(face, model);
		CreateTemplate(face, model, 0);
		cout << "TASK: " << taskIdentify << endl;
		break;
	case 1: // Control entry
		FaceCropImage(face, model);
		CreateTemplate(face, model, 1);
		break;
	case 3:
		FaceCropImage(face, model);
		CreateTemplate(face, model, 3);
		cout << "TASK: " << taskIdentify << endl;
		break;
	default:
		break;
	}
}

string Tracking::BuildHeadTracer() {
	char buff[20];	
	time_t now = time(NULL);
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));

	string timeInit(buff);	
	string tracerImage = timeInit + ", " + to_string(client) + ", frame, 1, ";
	return tracerImage;
}

void Tracking::CreateTemplate(void* face, Molded* model, int task) {
	int errorCode, templateSize;	
	
	float rightEyeX, rightEyeY, leftEyeX, leftEyeY;
	float faceConfidence;

	errorCode = IFACE_GetFaceBasicInfo(face, faceHandlerTracking,
		&rightEyeX, &rightEyeY, &leftEyeX, &leftEyeY, &faceConfidence);
	error->CheckError(errorCode, error->medium);

	string tracerImage = BuildHeadTracer() + 
		"(" + to_string(model->GetMoldCropWidth()) + "-" + to_string(model->GetMoldCropHeight()) + "), " + 
		to_string(faceConfidence) + ", " + 
		to_string(configurationIFaceProcessing->GetConfidenceThreshold()) + ", 1, ";


	errorCode = IFACE_CreateTemplate(face, faceHandlerTracking, 0, &templateSize, NULL);
	if (errorCode == IFACE_OK) {
		char* templateData = new char[templateSize];
		errorCode = IFACE_CreateTemplate(face, faceHandlerTracking, 0,
			&templateSize, templateData);
		if (errorCode != IFACE_OK) {
			error->CheckError(errorCode, error->medium);
		}
		else
		{			
			int majorVersion, minorVersion, quality;
			errorCode = IFACE_GetTemplateInfo(faceHandlerTracking,
				templateData, &majorVersion, &minorVersion, &quality);
			//cout << "QUALITY: " << quality << " CONFIGURATION QUALITY: " << configurationIFaceProcessing->GetQualityModel() << endl;
			tracerImage += to_string(quality) + ", " + to_string(configurationIFaceProcessing->GetQualityModel()) + ", ";
			if (quality > configurationIFaceProcessing->GetQualityModel()) {
				tracerImage += "1, ";
				int sizeImage[6]; 

				sizeImage[0] = model->GetMoldCropWidth();
				sizeImage[1] = model->GetMoldCropHeight();
				sizeImage[2] = templateSize;
				sizeImage[3] = quality;
				sizeImage[4] = task;
				sizeImage[5] = 0;

				auto tupleTemplateFace = std::make_tuple(templateData,
					model->GetCropImageData(), sizeImage, tracerImage);
				
				shootFace.on_next(tupleTemplateFace);
				
			}
			else {
				tracerImage += "0\n";
				file->WriteFile(tracerImage);
								
			}
		}
		delete[] templateData;
	}
	else {
		error->CheckError(errorCode, error->medium);
	}
	
}

void Tracking::FaceCropImage(void* face, Molded* model) {
	int cropWidth = -1, cropHeight = -1, cropLength = -1, errorCode = -1;

	switch (configurationIFaceProcessing->GetGetFaceCropImage())
	{
	case 0:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
			IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, NULL);
		error->CheckError(errorCode, error->medium);
		break;
	case 1:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
			IFACE_FACE_CROP_METHOD_FULL_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, NULL);
		error->CheckError(errorCode, error->medium);
		break;
	case 2:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
			IFACE_FACE_CROP_METHOD_FULL_FRONTAL_EXTENDED,
			&cropWidth, &cropHeight, &cropLength, NULL);
		error->CheckError(errorCode, error->medium);
		break;
	case 3:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
			IFACE_FACE_CROP_METHOD_TOKEN_NOT_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, NULL);
		error->CheckError(errorCode, error->medium);
		break;
	case 4:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
			IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL_EXTENDED,
			&cropWidth, &cropHeight, &cropLength, NULL);
		error->CheckError(errorCode, error->medium);
		break;
	default:
		break;
	}

	unsigned char* cropImageData = new unsigned char[cropLength];

	switch (configurationIFaceProcessing->GetGetFaceCropImage())
	{
	case 0:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
			IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, cropImageData);
		error->CheckError(errorCode, error->medium);
		break;
	case 1:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
			IFACE_FACE_CROP_METHOD_FULL_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, cropImageData);
		error->CheckError(errorCode, error->medium);
		break;
	case 2:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
			IFACE_FACE_CROP_METHOD_FULL_FRONTAL_EXTENDED,
			&cropWidth, &cropHeight, &cropLength, cropImageData);
		error->CheckError(errorCode, error->medium);
		break;
	case 3:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
			IFACE_FACE_CROP_METHOD_TOKEN_NOT_FRONTAL,
			&cropWidth, &cropHeight, &cropLength, cropImageData);
		error->CheckError(errorCode, error->medium);
		break;
	case 4:
		errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
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

void Tracking::CreateFaceOfObject(int indexTracking) {
	int errorCode;	
	void* face = nullptr;	
	
	//IFACE_TRACKED_OBJECT_FACE_TYPE_LAST_DISCOVERY
	//IFACE_TRACKED_OBJECT_FACE_TYPE_BEST_DISCOVERY
	//IFACE_TRACKED_OBJECT_FACE_TYPE_LAST    IFACE_PARAMETER_TRACK_DEEP_TRACK = true	

	errorCode = IFACE_CreateFace(&face);

	errorCode = IFACE_GetFaceFromObject(objects[indexTracking],
		objectHandler, face, IFACE_TRACKED_OBJECT_FACE_TYPE_BEST_DISCOVERY);
	error->CheckError(errorCode, error->medium);	
	
	if (errorCode == IFACE_OK) {		
		Molded* model = new Molded();
				
		RunTask(face, model);

		delete model;
				
	}
	errorCode = IFACE_ReleaseEntity(face); 
	error->CheckError(errorCode, error->medium);
	
}

void Tracking::TrackObjectState() {
	int errorCode; // countDesolation = 0;

	for (int trackedObjectIndex = 0; trackedObjectIndex < NUM_TRACKED_OBJECTS;
		trackedObjectIndex++)
	{

		float bbX, bbY, bbWidth, bbHeight;
		IFACE_TrackedObjectState trackedState;

		errorCode = IFACE_GetObjectState(objects[trackedObjectIndex],
			objectHandler, &trackedState);
		error->CheckError(errorCode, error->medium);
		if (trackedState == IFACE_TRACKED_OBJECT_STATE_CLEAN) {

			errorCode = IFACE_CleanObject(objects[trackedObjectIndex], objectHandler);
			error->CheckError(errorCode, error->medium);
			continue;
		}

		switch (trackedState)
		{
		case IFACE_TRACKED_OBJECT_STATE_TRACKED:
			
			float objectScore;
			errorCode = IFACE_GetObjectBoundingBox(objects[trackedObjectIndex],
				objectHandler, &bbX, &bbY, &bbWidth, &bbHeight);

			BuildCoordinatesImage(bbX, bbY, bbWidth, bbHeight, trackedObjectIndex);
			errorCode = IFACE_GetObjectScore(objects[trackedObjectIndex],
				objectHandler, &objectScore);
			error->CheckError(errorCode, error->medium);
			SetColorRectangle(objectScore, trackedObjectIndex);
			
			break;
		case IFACE_TRACKED_OBJECT_STATE_COVERED:
			errorCode = IFACE_GetObjectBoundingBox(objects[trackedObjectIndex],
				objectHandler, &bbX, &bbY, &bbWidth, &bbHeight);
			error->CheckError(errorCode, error->medium);
			BuildCoordinatesImage(bbX, bbY, bbWidth, bbHeight, trackedObjectIndex);
			break;
		case IFACE_TRACKED_OBJECT_STATE_SUSPEND:
			ClearCoordinatesImage(trackedObjectIndex);
			errorCode = IFACE_CleanObject(objects[trackedObjectIndex], objectHandler);
			error->CheckError(errorCode, error->medium);
			break;
		case IFACE_TRACKED_OBJECT_STATE_LOST:
			ClearCoordinatesImage(trackedObjectIndex);
			errorCode = IFACE_CleanObject(objects[trackedObjectIndex], objectHandler);
			error->CheckError(errorCode, error->medium);
			void* newObj;
			errorCode = IFACE_CreateObject(&newObj);
			objects[trackedObjectIndex] = newObj;
						

			break;
		case IFACE_TRACKED_OBJECT_STATE_CLEAN:
			
			break;
		}

		if (trackedState == IFACE_TRACKED_OBJECT_STATE_TRACKED) {	
			if (taskIdentify == ENROLL_VIDEO || 
				taskIdentify == IMPORT_VIDEO || 
				taskIdentify == CONTROL_ENTRY)
			{
				SendEnrollment(trackedObjectIndex);
			}
			/*else {
				std::thread tcf(&Tracking::CreateFaceOfObject, this, trackedObjectIndex);
				tcf.detach();
			}	*/								       			
		}
	}
	countFrameTracking++;
}

void Tracking::SendEnrollment(int objectIndex) {
	clock_t duration = clock() - timeStartEnroll;
	int durationMs = int(1000 * ((float)duration) / CLOCKS_PER_SEC);
	if (durationMs >= LAPSE_ENROLL_MS)
	{
		timeStartEnroll = clock();
		std::thread tcf(&Tracking::CreateFaceOfObject, this, objectIndex);
		tcf.detach();
	}
}

unsigned char* Tracking::LoadImageOfMemory(vector<unsigned char> buffer,
	int* width, int* height) {
	int lenght, errorCode;
	const char* imgData = reinterpret_cast<const char*> (&buffer[0]);
	if (imgData == NULL) {
		return NULL;
	}

	errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
		height, &lenght, NULL);
	if (errorCode != IFACE_OK) {
		
		return NULL;
	}

	unsigned char* rawImage = new unsigned char[lenght];
	errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
		height, &lenght, rawImage);
	if (errorCode != IFACE_OK) {
		
		return NULL;
	}

	return rawImage;

}

std::string Tracking::IntToStr(int num)
{
	std::ostringstream s;
	s << num;
	return s.str();
}

float* Tracking::GetCoordiantesRectangle() {
	return imageCoordinatesFollowed;
}

float* Tracking::GetColorRectangle() {
	return colorRectangle;
}

void Tracking::TerminateTracking() {
	int errorCode;

	for (int i = 0; i < NUM_TRACKED_OBJECTS; i++)
	{
		try
		{
			if (objects[i] != NULL)
			{
				errorCode = IFACE_ReleaseEntity(objects[i]);
				error->CheckError(errorCode, error->less);
			}

		}
		catch (const std::exception& e)
		{
			cout << e.what() << endl;
		}

	}

	errorCode = IFACE_ReleaseEntity(objectHandler);
	error->CheckError(errorCode, error->less);

	errorCode = IFACE_ReleaseEntity(faceHandlerTracking);
	error->CheckError(errorCode, error->less);
		
}

void Tracking::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
		});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
			shootError.on_next(either);
		});	

}