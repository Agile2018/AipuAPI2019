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
	void SetStringJSONTracking(string stringJson);
	
	void SetFrameView(FrameView* frame);
	ConfigurationFlowVideo* configuration = new ConfigurationFlowVideo();	
	void SetFinishLoop();	
	void StatePlay();
	void StatePaused();
	void SetIndexFrame(int value);
	
	int GetIndexFrame();

	void SetTaskIdentify(int value) {
		tracking->SetTaskIdentify(value);
	}	

	int GetTaskIdentify() {
		return tracking->GetTaskIdentify();
	}

	void SetConfigurationIFace(ConfigurationIFace* config) {
		tracking->SetConfigurationIFace(config);
	}
	

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();

	Rx::subject<std::tuple<char*, vector<unsigned char>, int*, string>> trackedFace;
	Rx::observable<std::tuple<char*, vector<unsigned char>, int*, string>> observableTrackedFace = trackedFace.get_observable();

private:
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	Rx::subscriber<std::tuple<char*, vector<unsigned char>, int*, string>> shootFace = trackedFace.get_subscriber();
	int indexFrame = 0;
	bool isInitFrame = false;	
	bool flagRate = false;
	Tracking* tracking = new Tracking();	
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
	string ConvertBackslashToSlash(string input);
};


#endif // !FLOWVIDEO_H

