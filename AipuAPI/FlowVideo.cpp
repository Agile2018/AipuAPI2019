#include "FlowVideo.h"
#include "Backrest.h"
#include "GBuffer.h"
#include "Video.h"
#include "ThreadPool.h"
#include <random>
#include <windows.h>
#include <winuser.h>

std::atomic<cv::Mat*> atomicFrame;
std::atomic<GBuffer*> atomicBuffer;
void* objectHandler;
void* faceHandlerTracking;
void* objects[NUM_TRACKED_OBJECTS];
double colorRectangle[NUM_TRACKED_OBJECTS] = {};
float imageCoordinatesFollowed[COORDINATES_X_ALL_IMAGES];
Backrest* backRest = new Backrest();
int client = 1;
int sizeVideoStream = 0;
int sequenceFps = 30;                   // fps of video	
int timeDeltaMs = 1000 / sequenceFps;
int refreshInterval = 2000;
int countFrameTracking = 0;
unsigned long int framesTotal = 0;
unsigned long int countFlowFrames = 0;
string nameWindow = "frame";
bool flagFirstDetect = false;
bool flagTracking = false;
GstElement *pipeline;
Video* video = new Video();

ThreadPool* pool = new ThreadPool(2);
std::mutex mtx;
std::deque<GBuffer> frameQueue;

GMainLoop *loop;
cv::Mat frame;

Rx::subject<void*> faceSubject;
Rx::observable<void*> observableFace = faceSubject.get_observable();
Rx::subscriber<void*> shootFace = faceSubject.get_subscriber();

Rx::subject<bool> trackStateSubject;
Rx::observable<bool> observableTrackState = trackStateSubject.get_observable();
Rx::subscriber<bool> shootTrackState = trackStateSubject.get_subscriber();

FlowVideo::FlowVideo()
{
	
}

FlowVideo::~FlowVideo()
{
	/*TerminateITracking();*/
	pool->shutdown();
	delete backRest;
}

void InitFlowVideo() {
	video->SetFlagInit(false);
	video->SetVideoIsDestroy(false);
	video->SetDestroy(false);
	countFlowFrames = 0;
}
void FlowVideo::SetFramesTotal(unsigned long int total) {
	framesTotal = total;
}

void FlowVideo::SetNameWindow(string name) {
	nameWindow = name;
	video->SetNameWindow(name);
}

void FlowVideo::ObserverFace() {
	auto observerFaceManagement = observableFace.map([](void* face) {
		return face;
		});


	auto subscriptionErrorManagement = observerFaceManagement.subscribe([this](void* face) {
		backRest->ProcessFaceTracking(face, client);
		
		});

	/*auto observerTrackStateManagement = observableTrackState.map([](bool result) {
		return result;
		});


	auto subscriptionErrorTrackStateManagement = observerTrackStateManagement.subscribe([this](bool result) {
		TrackObjectState();});*/
}

void FlowVideo::ObserverEvent() {
	auto observerErrorManagement = backRest->observableError.map([](Either* either) {
		return either;
	});


	auto subscriptionErrorManagement = observerErrorManagement.subscribe([this](Either* either) {
		string messageError = to_string(either->GetCode()) + ": " + either->GetLabel();
		shootError.on_next(messageError);
	});

	auto observerDatabase = backRest->observableUserJSON.map([](string jsonUser) {
		return jsonUser;
	});

	auto subscriptionDatabase = observerDatabase.subscribe([this](string jsonUser) {
		
		shootUserJSON.on_next(jsonUser);
		
	});

	subscriptionDatabase.clear();
}

void FlowVideo::SetClient(int value) {
	client = value;
	
}

void FlowVideo::SetIsRegister(bool option) {
	backRest->SetIsRegister(option);
}

void FlowVideo::ReloadRecognitionFace() {
	backRest->ReloadRecognitionFace();
}

void FlowVideo::SetConfigurationDatabase() {
	backRest->SetConfigurationDatabase();
}

void FlowVideo::SetRefreshInterval(int value) {
	refreshInterval = value;
}

void FlowVideo::SetSequenceFps(int value) {
	sequenceFps = value;
	timeDeltaMs = 1000 / sequenceFps;
}

void AdvanceVideoStream() {
	
	int positionVideoStream = (countFrameTracking / sizeVideoStream) + 1;
	int positionFrameMaxVideoStream = sizeVideoStream * positionVideoStream;

	if (countFrameTracking < positionFrameMaxVideoStream)
	{
		countFrameTracking = positionFrameMaxVideoStream;
	}
}

void ClearCoordinatesImage(int indexTracked) {
	int index = indexTracked * NUM_COORDINATES_X_IMAGE;
	imageCoordinatesFollowed[index] = 0;
	imageCoordinatesFollowed[index + 1] = 0;
	imageCoordinatesFollowed[index + 2] = 0;
	imageCoordinatesFollowed[index + 3] = 0;
}

void BuildCoordinatesImage(float x, float y, float width, float height, int indexTracked) {
	int index = indexTracked * NUM_COORDINATES_X_IMAGE;
	imageCoordinatesFollowed[index] = x;
	imageCoordinatesFollowed[index + 1] = y;
	imageCoordinatesFollowed[index + 2] = width;
	imageCoordinatesFollowed[index + 3] = height;
}

void SetColorRectangle(float score, int indexObject) {
	double percentageScore = ((double)score * 100.0) / 10000.0;
	double variationColor = (255.0 * percentageScore) / 100.0;
	double color = (255.0 - variationColor);
	colorRectangle[indexObject] = color;
}

void ClearAllCoordinatesImage() {
	for (int i = 0; i < NUM_TRACKED_OBJECTS; i++) {
		ClearCoordinatesImage(i);
	}
}

void ResetCoordinates() {
	long countFrames = countFrameTracking - 1;
	int residue = countFrames % sizeVideoStream;
	if (residue == 0)
	{
		ClearAllCoordinatesImage();
	}
}

void DrawRectangles(cv::Mat workMat) {
	static bool flagPaint[NUM_TRACKED_OBJECTS] = {};
	static int area[NUM_TRACKED_OBJECTS] = {};

	for (int i = 0; i < COORDINATES_X_ALL_IMAGES; i += 4)
	{
		if (imageCoordinatesFollowed[i] != 0) {
			int x = (int)imageCoordinatesFollowed[i];
			int y = (int)imageCoordinatesFollowed[i + 1];
			int w = (int)imageCoordinatesFollowed[i + 2];
			int h = (int)imageCoordinatesFollowed[i + 3];
			double c = colorRectangle[i];

			int horizontal[6] = {};
			int vertical[6] = {};
			int segmentWidth = w / 5;
			int segmentHeight = h / 5;
			for (int i = 0; i < 6; i++)
			{
				if (i == 0)
				{
					horizontal[i] = x;
				}
				else {
					horizontal[i] = horizontal[i - 1] + segmentWidth;
				}

			}

			for (int i = 0; i < 6; i++)
			{
				if (i == 0)
				{
					vertical[i] = y;
				}
				else {
					vertical[i] = vertical[i - 1] + segmentHeight;
				}

			}

			cv::Point p1u(horizontal[0], vertical[0]), p2u(horizontal[1], vertical[0]),
				p3u(horizontal[2], vertical[0]), p4u(horizontal[3], vertical[0]),
				p5u(horizontal[4], vertical[0]), p6u(horizontal[5], vertical[0]);

			cv::Point p1l(horizontal[0], vertical[0]), p2l(horizontal[0], vertical[1]),
				p3l(horizontal[0], vertical[2]), p4l(horizontal[0], vertical[3]),
				p5l(horizontal[0], vertical[4]), p6l(horizontal[0], vertical[5]);

			cv::Point p1d(horizontal[0], vertical[0] + h), p2d(horizontal[1], vertical[0] + h),
				p3d(horizontal[2], vertical[0] + h), p4d(horizontal[3], vertical[0] + h),
				p5d(horizontal[4], vertical[0] + h), p6d(horizontal[5], vertical[0] + h);

			cv::Point p1r(horizontal[0] + w, vertical[0]), p2r(horizontal[0] + w, vertical[1]),
				p3r(horizontal[0] + w, vertical[2]), p4r(horizontal[0] + w, vertical[3]),
				p5r(horizontal[0] + w, vertical[4]), p6r(horizontal[0] + w, vertical[5]);

			if (area[i] != (w * h))
			{
				flagPaint[i] = !flagPaint[i];
				area[i] = w * h;
			}

			if (flagPaint[i])
			{
				cv::line(workMat, p1u, p2u, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p3u, p4u, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p5u, p6u, cv::Scalar(255.0, 255.0, c), 2);

				cv::line(workMat, p1l, p2l, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p3l, p4l, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p5l, p6l, cv::Scalar(255.0, 255.0, c), 2);

				cv::line(workMat, p1d, p2d, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p3d, p4d, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p5d, p6d, cv::Scalar(255.0, 255.0, c), 2);

				cv::line(workMat, p1r, p2r, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p3r, p4r, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p5r, p6r, cv::Scalar(255.0, 255.0, c), 2);
			}
			else {
				cv::line(workMat, p2u, p3u, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p4u, p5u, cv::Scalar(255.0, 255.0, c), 2);

				cv::line(workMat, p2l, p3l, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p4l, p5l, cv::Scalar(255.0, 255.0, c), 2);

				cv::line(workMat, p2d, p3d, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p4d, p5d, cv::Scalar(255.0, 255.0, c), 2);

				cv::line(workMat, p2r, p3r, cv::Scalar(255.0, 255.0, c), 2);
				cv::line(workMat, p4r, p5r, cv::Scalar(255.0, 255.0, c), 2);
			}


		}
		
	}

}

void FlowVideo::LoadConfiguration(string nameFile) {
	pool->init();
	ObserverEvent();	
	ObserverFace();
	backRest->LoadConfiguration(nameFile);
	//gst_init(NULL, NULL);
}

void FlowVideo::LoadConnectionIdentify() {
	backRest->LoadConnectionIdentify();
}

unsigned char* LoadImageOfMemory(vector<unsigned char> buffer,
	int *width, int *height) {
	int lenght, errorCode;
	const char* imgData = reinterpret_cast<const char*> (&buffer[0]);
	if (imgData == NULL) {
		return NULL;
	}

	errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
		height, &lenght, NULL);
	if (errorCode != IFACE_OK) {
		//error->CheckError(errorCode, error->medium);
		return NULL;
	}

	unsigned char* rawImage = new unsigned char[lenght];
	errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
		height, &lenght, rawImage);
	if (errorCode != IFACE_OK) {
		//error->CheckError(errorCode, error->medium);
		return NULL;
	}
	
	return rawImage;

}

void CreateFaceOfObject(int indexTracking) {
	int errorCode;
	void* face;
	
	//IFACE_TRACKED_OBJECT_FACE_TYPE_LAST_DISCOVERY
	//IFACE_TRACKED_OBJECT_FACE_TYPE_BEST_DISCOVERY
	//IFACE_TRACKED_OBJECT_FACE_TYPE_LAST    IFACE_PARAMETER_TRACK_DEEP_TRACK = true

	errorCode = IFACE_CreateFace(&face);
	errorCode = IFACE_GetFaceFromObject(objects[indexTracking],
		objectHandler, face, IFACE_TRACKED_OBJECT_FACE_TYPE_BEST_DISCOVERY);

	if (face != NULL)
	{
		shootFace.on_next(face);
	}
	
}

void TrackObjectState() {
	int errorCode, countDesolation = 0;

	//pool->submit(ClearAllCoordinatesImage);

	for (int trackedObjectIndex = 0; trackedObjectIndex < NUM_TRACKED_OBJECTS;
		trackedObjectIndex++)
	{

		float bbX, bbY, bbWidth, bbHeight;
		IFACE_TrackedObjectState trackedState;

		errorCode = IFACE_GetObjectState(objects[trackedObjectIndex],
			objectHandler, &trackedState);

		if (trackedState == IFACE_TRACKED_OBJECT_STATE_CLEAN) {


			countDesolation++;
			if (countDesolation == NUM_TRACKED_OBJECTS && !flagFirstDetect)
			{
				AdvanceVideoStream();
			}
			errorCode = IFACE_CleanObject(objects[trackedObjectIndex], objectHandler);
			
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
			SetColorRectangle(objectScore, trackedObjectIndex);
			
			//printf("   face id is tracked. Its bounding box :(%f, %f), (%f, %f), Face score : , Object score : \n", bbX, bbY, bbWidth, bbHeight);
			break;
		case IFACE_TRACKED_OBJECT_STATE_COVERED:
			errorCode = IFACE_GetObjectBoundingBox(objects[trackedObjectIndex],
				objectHandler, &bbX, &bbY, &bbWidth, &bbHeight);

			BuildCoordinatesImage(bbX, bbY, bbWidth, bbHeight, trackedObjectIndex);
			break;
		case IFACE_TRACKED_OBJECT_STATE_SUSPEND:
			ClearCoordinatesImage(trackedObjectIndex);
			errorCode = IFACE_CleanObject(objects[trackedObjectIndex], objectHandler);
			//printf("STATE SUSPEND INDEX: %d\n", trackedObjectIndex);

			break;
		case IFACE_TRACKED_OBJECT_STATE_LOST:
			ClearCoordinatesImage(trackedObjectIndex);
			errorCode = IFACE_CleanObject(objects[trackedObjectIndex], objectHandler);
			void* newObj;
			errorCode = IFACE_CreateObject(&newObj);
			objects[trackedObjectIndex] = newObj;
			flagFirstDetect = false;
			//printf("STATE LOST INDEX: %d\n", trackedObjectIndex);

			break;
		case IFACE_TRACKED_OBJECT_STATE_CLEAN:
			//printf("STATE LOST CLEAN OBJECT INDEX: %d\n", trackedObjectIndex);

			break;
		}

		if (trackedState == IFACE_TRACKED_OBJECT_STATE_TRACKED) {
			std::thread tcf(CreateFaceOfObject, trackedObjectIndex);
			tcf.detach();
		}
	}
	countFrameTracking++;
}

void FaceTracking(std::vector<unsigned char> vectorData) {
	int width, height, errorCode; 

	//clock_t timeStart2 = clock();
	unsigned char* rawImageData = LoadImageOfMemory(vectorData, &width, &height);		
	/*clock_t duration2 = clock() - timeStart2;
	int durationMs2 = int(1000 * ((float)duration2) / CLOCKS_PER_SEC);	
	printf("   Loading Image time: %d \n", durationMs2);*/

	if (rawImageData != NULL) {
		long secuence = countFrameTracking * timeDeltaMs;
		ResetCoordinates();
		
		errorCode = IFACE_TrackObjects(objectHandler, rawImageData,
			width, height, secuence, NUM_TRACKED_OBJECTS, objects);
		/*clock_t duration1 = clock() - timeStart1;
		int durationMs1 = int(1000 * ((float)duration1) / CLOCKS_PER_SEC);		
		printf("   TRACKING OBJECT time: %d \n", durationMs1);*/

		//if (errorCode != IFACE_OK) {
		//	shootTrackState.on_next(true);
		//	
		//}			
		
		TrackObjectState();
		
		delete[] rawImageData;		
	}
	flagTracking = false;
	vectorData.clear();
		
}

void RunTracking(std::vector<uchar> buffer) {
	if (!flagTracking)
	{
		flagTracking = true;
		FaceTracking(buffer);
	}
}

void BackProcessImage(char* data, int size, int client) {
	backRest->ProcessImageInBack(data, size, client);
}

void SetAtomicFrame(char* data, int size) {

	if (mtx.try_lock()) {
		GBuffer* prevFrameBuffer = atomicBuffer.exchange(new GBuffer(data, size));
		if (prevFrameBuffer)
		{
			delete prevFrameBuffer;
		}
		mtx.unlock();
	}
}

GstFlowReturn NewSample(GstAppSink *appsink, gpointer /*data*/)
{
	
	if (framesTotal != 0 && (countFlowFrames + 1) == framesTotal)
	{
		video->SetDestroy(true);		
	}
	
	countFlowFrames++;

	GstSample *sample = gst_app_sink_pull_sample(appsink);
	if (sample == NULL) {
		return GST_FLOW_ERROR;
	}
	
	GstBuffer *buffer = gst_sample_get_buffer(sample);	

	GstMapInfo map;
	if (!gst_buffer_map((buffer), &map, GST_MAP_READ)) {
		gst_buffer_unmap((buffer), &map);
		gst_sample_unref(sample);
		return GST_FLOW_ERROR;
	}

	
	if (map.data != NULL && !video->GetVideoIsDestroy())
	{	

		std::vector<uchar> bufferMap((char*)map.data, (char*)map.data + (int)map.size);
		std::thread ttb(RunTracking, bufferMap);
		ttb.detach();

		try
		{
			//clock_t timeStart1 = clock();

			frame = cv::imdecode(bufferMap, cv::IMREAD_UNCHANGED);

			/*clock_t duration1 = clock() - timeStart1;
			int durationMs1 = int(1000 * ((float)duration1) / CLOCKS_PER_SEC);
			printf("   cv::imdecode conversion time: %d \n", durationMs1);*/

			if (!frame.empty())
				DrawRectangles(frame);
			video->Play(frame);
			/*cv::imshow(nameWindow.c_str(), frame);
			cv::waitKey(1);*/
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}	
	}
	gst_buffer_unmap(buffer, &map);
	gst_sample_unref(sample);	
	return GST_FLOW_OK;
}

gboolean MessageCallback(GstBus *bus, GstMessage *message, gpointer data)
{

	switch (GST_MESSAGE_TYPE(message)) {
	case GST_MESSAGE_ERROR: {
		GError *err;
		gchar *debug;

		gst_message_parse_error(message, &err, &debug);
		g_print("Error: %s\n", err->message);
		g_error_free(err);
		g_free(debug);
		break;
	}
	case GST_MESSAGE_EOS:
		/* end-of-stream */		
		//g_main_loop_quit(loop);		
		cout << "end-of-stream" << endl;
		break;
	default:
		/* unhandled message */
		cout << "DEFAULT" << endl;
		break;
	}

	return true;
}


std::string IntToStr(int num)
{	
	std::ostringstream s;
	s << num;
	return s.str();
}

void FlowVideo::RecognitionFaceFiles(string file, int client) {
	backRest->RecognitionFaceFiles(file, client);
}

void FlowVideo::SetIsFinishLoadFiles(bool value) {
	backRest->SetIsFinishLoadFiles(value);
}

bool FlowVideo::GetIsFinishLoadFiles() {
	return backRest->GetIsFinishLoadFiles();
}

void FlowVideo::InitITracking() {
	int errorCode;

	errorCode = IFACE_CreateFaceHandler(&faceHandlerTracking);
	error->CheckError(errorCode, error->medium);
	errorCode = IFACE_CreateObjectHandler(&objectHandler, faceHandlerTracking);
	error->CheckError(errorCode, error->medium);

	for (int i = 0; i < NUM_TRACKED_OBJECTS; i++)
	{
		errorCode = IFACE_CreateObject(&objects[i]);
		error->CheckError(errorCode, error->medium);
	}

	errorCode = IFACE_SetParam(objectHandler,
		IFACE_PARAMETER_TRACK_FACE_DISCOVERY_FREQUENCE_MS,
		IntToStr(refreshInterval).c_str());
	error->CheckError(errorCode, error->medium);
	errorCode = IFACE_SetParam(objectHandler,
		IFACE_PARAMETER_TRACK_MIN_EYE_DISTANCE,
		IntToStr(minEyeDistance).c_str());
	error->CheckError(errorCode, error->medium);
	errorCode = IFACE_SetParam(objectHandler,
		IFACE_PARAMETER_TRACK_MAX_EYE_DISTANCE,
		IntToStr(maxEyeDistance).c_str());
	error->CheckError(errorCode, error->medium);
	errorCode = IFACE_SetParam(faceHandlerTracking,
		IFACE_PARAMETER_FACEDET_CONFIDENCE_THRESHOLD,
		IntToStr(faceConfidenceThresh).c_str()); //
	error->CheckError(errorCode, error->medium);

	if (trackingMode == 0)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_TRACKING_MODE,
			IFACE_TRACK_TRACKING_MODE_DEFAULT);
		error->CheckError(errorCode, error->medium);
	}
	if (trackingMode == 1)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_TRACKING_MODE,
			IFACE_TRACK_TRACKING_MODE_LIVENESS_DOT);
		error->CheckError(errorCode, error->medium);
	}
	if (trackingMode == 2)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_TRACKING_MODE,
			IFACE_TRACK_TRACKING_MODE_OBJECT_TRACKING);
		error->CheckError(errorCode, error->medium);
	}

	if (trackSpeed == 0)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_SPEED_ACCURACY_MODE,
			IFACE_TRACK_SPEED_ACCURACY_MODE_ACCURATE);
		error->CheckError(errorCode, error->medium);
	}
	
	if (trackSpeed == 1)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_SPEED_ACCURACY_MODE,
			IFACE_TRACK_SPEED_ACCURACY_MODE_BALANCED);
		error->CheckError(errorCode, error->medium);
	}

	if (trackSpeed == 2)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_SPEED_ACCURACY_MODE,
			IFACE_TRACK_SPEED_ACCURACY_MODE_FAST);
		error->CheckError(errorCode, error->medium);
	}

	if (motionOptimization == 0)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
			IFACE_TRACK_MOTION_OPTIMIZATION_DISABLED);
		error->CheckError(errorCode, error->medium);
	}
	
	if (motionOptimization == 1)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
			IFACE_TRACK_MOTION_OPTIMIZATION_HISTORY_LONG_ACCURATE);
		error->CheckError(errorCode, error->medium);
	}
	if (motionOptimization == 2)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
			IFACE_TRACK_MOTION_OPTIMIZATION_HISTORY_LONG_FAST);
		error->CheckError(errorCode, error->medium);
	}
	if (motionOptimization == 3)
	{
		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
			IFACE_TRACK_MOTION_OPTIMIZATION_HISTORY_SHORT);
		error->CheckError(errorCode, error->medium);
	}

	errorCode = IFACE_SetParam(objectHandler,
		IFACE_PARAMETER_TRACK_DEEP_TRACK,
		deepTrack.c_str()); // IFACE_TRACK_DEEP_TRACK_DEFAULT
	error->CheckError(errorCode, error->medium);
		
	flagTracking = false;
	flagFirstDetect = false;

	sizeVideoStream = refreshInterval / timeDeltaMs;

	InitFlowVideo();
}

void FlowVideo::CaptureFlow(int optionFlow) {
	

	InitITracking();
	
	gchar *descr = DescriptionFlow(optionFlow);
	
	
	gst_init(NULL, NULL);
	
	GError *gError = nullptr;	
	pipeline = gst_parse_launch(descr, &gError);
	if (gError) {
		g_print("could not construct pipeline: %s\n", gError->message);
		g_error_free(gError);
		exit(-1);
	}

	GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");

	gst_app_sink_set_emit_signals((GstAppSink*)sink, true);
	gst_app_sink_set_drop((GstAppSink*)sink, true);
	gst_app_sink_set_max_buffers((GstAppSink*)sink, 1);
		
	GstAppSinkCallbacks* appsink_callbacks = (GstAppSinkCallbacks*)malloc(sizeof(GstAppSinkCallbacks));
	appsink_callbacks->eos = NULL;
	appsink_callbacks->new_preroll = NULL;
	appsink_callbacks->new_sample = NewSample;
	gst_app_sink_set_callbacks(GST_APP_SINK(sink), appsink_callbacks, (gpointer)NULL, free);
	
	GstBus *bus;
	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	gst_bus_add_watch(bus, MessageCallback, nullptr);
	gst_object_unref(bus);

	//cv::namedWindow(nameWindow.c_str(), cv::WINDOW_NORMAL); //cv::WINDOW_GUI_EXPANDED
	//cv::moveWindow(nameWindow.c_str(), 1200, 100);
	GstStateChangeReturn ret;
	ret = gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE) {
		g_printerr("Unable to set the pipeline to the playing state.\n");
		gst_object_unref(pipeline);
		exit(-1);
	}
	
	loop = g_main_loop_new(NULL, false);
	g_main_loop_run(loop);
		

	gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
	gst_object_unref(GST_OBJECT(pipeline));
	
	ClearAllCoordinatesImage();
	countFrameTracking = 0;	
	TerminateITracking();
}

void FlowVideo::TerminateITracking() {
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

gchar* FlowVideo::DescriptionFlow(int optionFlow) {
	gchar *descr = nullptr;

	switch (optionFlow) {
	case 1: // IP CAMERA
		descr = g_strdup_printf(
			"rtspsrc location=%s "
			"! application/x-rtp, payload=96 ! rtph264depay ! h264parse ! avdec_h264 "
			"! decodebin ! videoconvert "
			"! video/x-raw, format=(string)I420 "
			"! jpegenc "
			"! appsink name=sink emit-signals=true sync=false max-buffers=1 drop=true",
			ipCamera.c_str()
		);
		
		break;
	case 2: // FILE		
		descr = g_strdup_printf(
			"filesrc location=%s "
			"! decodebin ! videoconvert "			
			"! video/x-raw, format=(string)I420 "
			"! jpegenc quality=100 "
			"! appsink name=sink emit-signals=true sync=true max-buffers=1 drop=true",
			fileVideo.c_str()
		);
		
		break;
	case 3: // CAMERA   
		descr = g_strdup_printf(
			"v4l2src device=%s "
			"! decodebin ! videoconvert "			
			"! video/x-raw, format=(string)I420  "
			"! jpegenc "
			"! appsink name=sink emit-signals=true sync=true max-buffers=1 drop=true",
			deviceVideo.c_str()
		);		
		break;
	}
	return descr;

}

void FlowVideo::ShowWindow(int option) {
	GstState cur_state;
	
	gst_element_get_state(GST_ELEMENT(pipeline), &cur_state, NULL, 0);
	if (cur_state == GST_STATE_PLAYING) {
		std::wstring stemp = std::wstring(nameWindow.begin(), nameWindow.end());
		LPCWSTR sw = stemp.c_str();
		HWND win_handle = FindWindow(0, sw);
		switch (option)
		{
		case 1:
			SetWindowPos(win_handle, HWND_BOTTOM, 0, 0, 0, 0, wFlags);
			break;
		case -1:
			SetWindowPos(win_handle, HWND_TOPMOST, 0, 0, 0, 0, wFlags);
			break;
		}
	}
		
}

void FlowVideo::ResetLowScore() {
	backRest->ResetLowScore();
}

int FlowVideo::GetCountLowScore() {
	return backRest->GetCountLowScore();
}

void FlowVideo::ResetCountNotDetect() {
	backRest->ResetCountNotDetect();
}

int FlowVideo::GetCountNotDetect() {
	return backRest->GetCountNotDetect();
}

void FlowVideo::SetDeepTrack(string value) {
	deepTrack = value;
}

void FlowVideo::ResetCountRepeatUser() {
	backRest->ResetCountRepeatUser();
}

int FlowVideo::GetCountRepeatUser() {
	return backRest->GetCountRepeatUser();
}

void FlowVideo::StatePlay() {
	gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
}

void FlowVideo::StatePaused() {	
	gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PAUSED);
}

void FlowVideo::SetFlagFlow(bool flag) {
	GstState cur_state;
	video->SetDestroy(true);
	gst_element_get_state(GST_ELEMENT(pipeline), &cur_state, NULL, 0);

	if (cur_state == GST_STATE_PLAYING) {
		gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PAUSED);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	g_main_loop_quit(loop);
}