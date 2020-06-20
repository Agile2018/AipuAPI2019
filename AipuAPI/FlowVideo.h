#ifndef FLOWVIDEO_H
#define FLOWVIDEO_H

#include <iostream>
#include <gst/gst.h>
#include <gst/app/app.h>
#include <sstream>
#include <thread>
#include "ErrorAipuLib.h"
#include "Tracking.h"
#include "FrameView.h"
#include "ConfigurationFlowVideo.h"

using namespace std;

class FlowVideo
{
public:
	FlowVideo();
	~FlowVideo();

	void RunFlowVideo();
	void SetNameDirectoryTracking(string name);
	void SetNameFileConfigurationTracking(string name);
	void SetFrameView(FrameView* frame);
	ConfigurationFlowVideo* configuration = new ConfigurationFlowVideo();
	//void Terminate();
	//void LoadConfiguration(string nameFile);	

	/*void SetMinEyeDistance(int value) {
		minEyeDistance = value;
	}

	void SetMaxEyeDistance(int value) {
		maxEyeDistance = value;
	}

	void SetFaceConfidenceThresh(int value) {
		faceConfidenceThresh = value;
	}

	void SetRefreshInterval(int value);

	void SetSequenceFps(int value);*/

	/*void SetIpCamera(string ip) {
		ipCamera = ip;
	}

	void SetDeviceVideo(string device) {
		deviceVideo = device;
	}*/

	/*void SetTrackingMode(int mode) {
		trackingMode = mode;
	}

	void SetTrackSpeed(int speed) {
		trackSpeed = speed;
	}

	void SetMotionOptimization(int motion) {
		motionOptimization = motion;
	}*/

	/*void SetVideoScaleMethod(int value) {
		videoScaleMethod = value;
	}*/

	/*void SetWidthFrame(int value) {
		widthFrame = value;
	}

	void SetHeightFrame(int value) {
		heightFrame = value;
	}*/

	/*void SetFileVideo(string file) {
		fileVideo = file;
	}*/

	//void SetNameWindow(string name);

	/*void SetFlagFlow(bool flag) {
		flagFlow = flag;
	}*/
	void SetFinishLoop();
	//void SetClient(int value);

	//void SetIsRegister(bool option);

	//void ShowWindow(int option);

	//void ReloadRecognitionFace();

	//void SetConfigurationDatabase();

	//void RecognitionFaceFiles(string file, int client);

	//void SetIsFinishLoadFiles(bool value);
	//bool GetIsFinishLoadFiles();
	//void ResetLowScore();
	//int GetCountLowScore();
	//void ResetCountNotDetect();
	//int GetCountNotDetect();
	//void SetDeepTrack(string value);
	//void ResetCountRepeatUser();
	//int GetCountRepeatUser();
	//void LoadConnectionIdentify();
	void StatePlay();
	void StatePaused();
	void SetIndexFrame(int value);

	int GetIndexFrame();

	//void SetFramesTotal(unsigned long int total);

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();

	Rx::subject<std::tuple<char*, vector<unsigned char>, int*>> trackedFace;
	Rx::observable<std::tuple<char*, vector<unsigned char>, int*>> observableTrackedFace = trackedFace.get_observable();

private:
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	Rx::subscriber<std::tuple<char*, vector<unsigned char>, int*>> shootFace = trackedFace.get_subscriber();
	int indexFrame = 0;
	bool isInitFrame = false;	
	bool flagRate = false;
	Tracking* tracking = new Tracking();
	//FrameView* frameView;
	//Rx::subscriber<string> shootUserJSON = userDetected.get_subscriber();
	//int minEyeDistance = 20;          // minimal eye distance in input image
	//int maxEyeDistance = 200;         // maximal eye distance in input image
	//int faceConfidenceThresh = 450;         // face detection confidence threshold		
	//int trackingMode = 1;
	//int trackSpeed = 2;
	//int motionOptimization = 2;
	/*string ipCamera;
	string fileVideo;
	string deviceVideo = "/dev/video0";*/
	//string deepTrack = "true";
	//int videoScaleMethod = 1; //3 1
	/*int widthFrame = 640;
	int heightFrame = 480;		
	bool flagFlow = false;	*/

	/*const int SWP_NOSIZE = 0x1;
	const int SWP_NOMOVE = 0x2;
	const int SWP_NOACTIVATE = 0x10;
	const int wFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE;
	const int HWND_TOPMOST = -1;
	const int HWND_BOTTOM = 1;*/

	//ErrorFaceLib* error = new ErrorFaceLib();
	
	/*void InitITracking();
	void TerminateITracking();*/
	gchar* DescriptionFlow(int optionFlow);
	GstElement* pipeline = nullptr;
	GMainLoop* loop = nullptr;
	void ObserverError();
	void ObserverFace();
	void InitParamsFrame(std::tuple<int, int, int, int> tupleParams);
	static GstFlowReturn CaptureGstBuffer(GstAppSink* appsink, gpointer /*data*/);
	static gboolean MessageCallback(GstBus* bus, GstMessage* message, gpointer data);
	void DrawRectangles();
	void RunTracking(std::vector<unsigned char> buffer, int index);
	void SendImageScreen(unsigned char* data, int size, int index);
	//void ObserverFace();
	//void TrackObjectState();
};


#endif // !FLOWVIDEO_H

