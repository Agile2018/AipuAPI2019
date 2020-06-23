#include "Tracking.h"

//unsigned long int countFlowFrames = 0;
//float colorRectangle[NUM_TRACKED_OBJECTS] = {};
//float imageCoordinatesFollowed[COORDINATES_X_ALL_IMAGES];

Tracking::Tracking()
{
	
	ObserverError();
}

Tracking::~Tracking()
{
	
}

void Tracking::InitITracking() {
	int errorCode;

	errorCode = IFACE_CreateFaceHandler(&faceHandlerTracking);
	error->CheckError(errorCode, error->medium);
	//////
	errorCode = IFACE_SetParam(faceHandlerTracking,
		IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
		IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_FAST);
	error->CheckError(errorCode, error->medium);

	errorCode = IFACE_SetParam(faceHandlerTracking,
		IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
		IFACE_FACEDET_SPEED_ACCURACY_MODE_FAST);
	error->CheckError(errorCode, error->medium);
	//////

	errorCode = IFACE_CreateObjectHandler(&objectHandler, faceHandlerTracking);
	error->CheckError(errorCode, error->medium);

	for (int i = 0; i < NUM_TRACKED_OBJECTS; i++)
	{
		errorCode = IFACE_CreateObject(&objects[i]);
		error->CheckError(errorCode, error->medium);
	}
	

	errorCode = IFACE_SetParam(objectHandler,
		IFACE_PARAMETER_TRACK_FACE_DISCOVERY_FREQUENCE_MS,
		IntToStr(configuration->GetRefreshInterval()).c_str());
	error->CheckError(errorCode, error->medium);
	errorCode = IFACE_SetParam(objectHandler,
		IFACE_PARAMETER_TRACK_MIN_FACE_SIZE,
		IntToStr(configuration->GetMinEyeDistance()).c_str());
	error->CheckError(errorCode, error->medium);
	errorCode = IFACE_SetParam(objectHandler,
		IFACE_PARAMETER_TRACK_MAX_FACE_SIZE,
		IntToStr(configuration->GetMaxEyeDistance()).c_str());
	error->CheckError(errorCode, error->medium);
	errorCode = IFACE_SetParam(faceHandlerTracking,
		IFACE_PARAMETER_FACEDET_CONFIDENCE_THRESHOLD,
		IntToStr(configuration->GetFaceConfidenceThresh()).c_str()); //
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

	if (configuration->GetDeepTrack() == 1)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_DEEP_TRACK,
			"true"); // IFACE_TRACK_DEEP_TRACK_DEFAULT
		error->CheckError(errorCode, error->medium);
	}

	if (configuration->GetDeepTrack() == 0)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_DEEP_TRACK,
			"false"); // IFACE_TRACK_DEEP_TRACK_DEFAULT
		error->CheckError(errorCode, error->medium);
	}

	flagTracking = false;
	flagFirstDetect = false;

	//sizeVideoStream = configuration->GetRefreshInterval() / configuration->GetTimeDeltaMs();
	//countFlowFrames = 0;
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
		TrackObjectState();

		delete[] rawImageData;
	}
	flagTracking = false;
	vectorData.clear();
}

void Tracking::AdvanceVideoStream() {
	int positionVideoStream = (countFrameTracking / sizeVideoStream) + 1;
	int positionFrameMaxVideoStream = sizeVideoStream * positionVideoStream;

	if (countFrameTracking < positionFrameMaxVideoStream)
	{
		countFrameTracking = positionFrameMaxVideoStream;
	}
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

void Tracking::CreateTemplate(void* face, Molded* model) {
	int errorCode, majorVersion, minorVersion, quality;
	int templateSize;
	
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
			errorCode = IFACE_GetTemplateInfo(faceHandlerTracking,
				templateData, &majorVersion, &minorVersion, &quality);
			if (quality > configuration->GetQualityModel()) {
				//model->SetMoldSize(templateSize);
				//model->SetMoldImage(templateData);
				int sizeImage[4];
				sizeImage[0] = model->GetMoldCropWidth();
				sizeImage[1] = model->GetMoldCropHeight();
				sizeImage[2] = templateSize;
				sizeImage[3] = quality;

				auto tupleTemplateFace = std::make_tuple(templateData,
					model->GetCropImageData(), sizeImage);
				
				shootFace.on_next(tupleTemplateFace);
				
			}

		}
		delete[] templateData;
	}
	else {
		error->CheckError(errorCode, error->medium);
	}
	
}

void Tracking::FaceCropImage(void* face, Molded* model) {
	int cropWidth, cropHeight, cropLength, errorCode;	

	errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
		IFACE_FACE_CROP_METHOD_FULL_FRONTAL,
		&cropWidth, &cropHeight, &cropLength, NULL);
	error->CheckError(errorCode, error->medium);
	unsigned char* cropImageData = new unsigned char[cropLength];
	errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
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
		FaceCropImage(face, model);		
		CreateTemplate(face, model);				
		delete model;
		/*if (errorCode == IFACE_OK) {

			cropSize[0] = cropWidth;
			cropSize[1] = cropHeight;
			cropSize[2] = cropLength;

			errorCode = IFACE_SerializeEntity(face, NULL,
				&serializedEntitySize);
			error->CheckError(errorCode, error->medium);

			char* serializedEntity = new char[serializedEntitySize];

			errorCode = IFACE_SerializeEntity(face, serializedEntity,
				&serializedEntitySize);
			error->CheckError(errorCode, error->medium);
			if (errorCode == IFACE_OK && serializedEntity != NULL) {
				cropSize[3] = serializedEntitySize;
				auto tupleSerializeFace = std::make_tuple(serializedEntity, 
					cropImageData, cropSize);
				shootFace.on_next(tupleSerializeFace);
			}
			else
			{
				delete[] serializedEntity;
				delete[] cropImageData;
			}
		}*/
		
	}
	errorCode = IFACE_ReleaseEntity(face); 
	error->CheckError(errorCode, error->medium);
	
}

void Tracking::TrackObjectState() {
	int errorCode, countDesolation = 0;

	for (int trackedObjectIndex = 0; trackedObjectIndex < NUM_TRACKED_OBJECTS;
		trackedObjectIndex++)
	{

		float bbX, bbY, bbWidth, bbHeight;
		IFACE_TrackedObjectState trackedState;

		errorCode = IFACE_GetObjectState(objects[trackedObjectIndex],
			objectHandler, &trackedState);
		error->CheckError(errorCode, error->medium);
		if (trackedState == IFACE_TRACKED_OBJECT_STATE_CLEAN) {


			countDesolation++;
			if (countDesolation == NUM_TRACKED_OBJECTS && !flagFirstDetect)
			{
				AdvanceVideoStream();
			}
			errorCode = IFACE_CleanObject(objects[trackedObjectIndex], objectHandler);
			error->CheckError(errorCode, error->medium);
			continue;
		}

		switch (trackedState)
		{
		case IFACE_TRACKED_OBJECT_STATE_TRACKED:
			flagFirstDetect = true;
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
			flagFirstDetect = false;			

			break;
		case IFACE_TRACKED_OBJECT_STATE_CLEAN:
			
			break;
		}

		if (trackedState == IFACE_TRACKED_OBJECT_STATE_TRACKED) {			
			std::thread tcf(&Tracking::CreateFaceOfObject, this, trackedObjectIndex);
			tcf.detach();
			       			
		}
	}
	countFrameTracking++;
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
	
	//errorCode = IFACE_ReleaseEntity(faceTrack); //faceHandler
	//error->CheckError(errorCode, error->medium);
}

void Tracking::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
		});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
			shootError.on_next(either);
		});	

}