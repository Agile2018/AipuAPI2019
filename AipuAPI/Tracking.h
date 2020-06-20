#ifndef TRACKING_H
#define TRACKING_H

#include <iostream>
#include <sstream>
#include <vector>
#include "ErrorFaceLib.h"
#include "ConfigurationTracking.h"
#include "Molded.h"

#define NUM_TRACKED_OBJECTS	5
#define COORDINATES_X_ALL_IMAGES 20
#define NUM_COORDINATES_X_IMAGE  4

using namespace std;

class Tracking
{
public:
	Tracking();
	~Tracking();
	void InitITracking();
	void FaceTracking(std::vector<unsigned char> vectorData);
	void TerminateTracking();	

	bool GetFlagTracking() {
		return flagTracking;
	}

	void SetFlagTracking(bool value) {
		flagTracking = value;
	}

	void SetCountFrameTracking(int value) {
		countFrameTracking = value;
	}

	void ReCalculateSizeVideoStream(int fps) {
		
		configuration->SetSequenceFps(fps);
		sizeVideoStream = configuration->GetRefreshInterval() / configuration->GetTimeDeltaMs();
		
	}

	int GetSequenceFps() {
		return configuration->GetSequenceFps();
	}

	void ClearAllCoordinatesImage();
	float* GetCoordiantesRectangle();
	float* GetColorRectangle();
	ConfigurationTracking* configuration = new ConfigurationTracking();
	Rx::subject<std::tuple<char*, vector<unsigned char>, int*>> faceSubject;
	Rx::observable<std::tuple<char*, vector<unsigned char>, int*>> observableTrackingFace = faceSubject.get_observable();
	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();

private:		
	long countFrameTracking = 0;
	int sizeVideoStream = 0;
	void* objectHandler = nullptr;
	void* faceHandlerTracking = nullptr;
	void* objects[NUM_TRACKED_OBJECTS] = {};	
	
	bool flagFirstDetect = false;
	bool flagTracking = false;
	float colorRectangle[NUM_TRACKED_OBJECTS] = {};
	float imageCoordinatesFollowed[COORDINATES_X_ALL_IMAGES];
	void SetColorRectangle(float score, int indexObject);
	void BuildCoordinatesImage(float x, float y, float width, float height, int indexTracked);
	void ClearCoordinatesImage(int indexTracked);
	ErrorFaceLib* error = new ErrorFaceLib();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	Rx::subscriber<std::tuple<char*, vector<unsigned char>, int*>> shootFace = faceSubject.get_subscriber();
	string IntToStr(int num);
	unsigned char* LoadImageOfMemory(vector<unsigned char> buffer,
		int* width, int* height);
	void CreateFaceOfObject(int indexTracking);
	void AdvanceVideoStream();	
	void ResetCoordinates();
	void TrackObjectState();		
	void ObserverError();
	void CreateTemplate(void* face, Molded* model);
	void FaceCropImage(void* face, Molded* model);
};

#endif // !TRACKING_H