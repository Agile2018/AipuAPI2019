#include "FlowVideo.h"

FrameView* frameView;
ErrorAipuLib* error = new ErrorAipuLib();

Rx::subject<std::tuple<int, int, int, int>> initFrameSubject;
Rx::observable<std::tuple<int, int, int, int>> observableInitFrame = initFrameSubject.get_observable();
Rx::subscriber<std::tuple<int, int, int, int>> shootInitFrame = initFrameSubject.get_subscriber();

Rx::subject<std::tuple<unsigned char*, int, int>> imageDataSubject;
Rx::observable<std::tuple<unsigned char*, int, int>> observableImageData = imageDataSubject.get_observable();
Rx::subscriber<std::tuple<unsigned char*, int, int>> shootImageData = imageDataSubject.get_subscriber();


FlowVideo::FlowVideo()
{
	ObserverError();
	ObserverFace();
}

FlowVideo::~FlowVideo()
{
			
	delete tracking;
	
}

void FlowVideo::SetIndexFrame(int value) {
	indexFrame = value;
}

int FlowVideo::GetIndexFrame() {
	return indexFrame;
}

void FlowVideo::SetNameFileConfigurationTracking(string name) {
	tracking->configuration->SetNameFileConfiguration(name);
	tracking->configuration->ParseJSONToObject();
}

void FlowVideo::SetNameDirectoryTracking(string name) {
	tracking->configuration->SetNameDirectory(name);
}

void FlowVideo::SetFrameView(FrameView* frame) {
	frameView = frame;
}

void FlowVideo::InitParamsFrame(std::tuple<int, int, int, int> tupleParams) {
	int index = std::get<3>(tupleParams);
	if (!isInitFrame && index == indexFrame)
	{
		isInitFrame = true;
		frameView->SetWidthImage(indexFrame, std::get<0>(tupleParams));
		frameView->SetHeightImage(indexFrame, std::get<1>(tupleParams));
		tracking->ReCalculateSizeVideoStream(std::get<2>(tupleParams));
		frameView->SetShowFrame(indexFrame, true);
	}
	
}

void FlowVideo::ObserverFace() {
	auto templateObservable = tracking->observableTrackingFace
		.map([](std::tuple<char*, vector<unsigned char>, int*> model) {
		return model;
	});

	auto subscriptionTemplate = templateObservable
		.subscribe([this](std::tuple<char*, vector<unsigned char>, int*> model) {

		shootFace.on_next(model);
		
	});

	subscriptionTemplate.clear();	

	auto frameInitObservable = observableInitFrame.map([](std::tuple<int, int, int, int> initFrame) {
		return initFrame;
		});

	auto subscriptionFrameInit = frameInitObservable.subscribe([this](std::tuple<int, int, int, int> initFrame) {

		InitParamsFrame(initFrame);

		});

	subscriptionFrameInit.clear();

	auto imageDataObservable = observableImageData.map([](std::tuple<unsigned char*, int, int> dataImg) {
		return dataImg;
		});

	auto subscriptionImageData = imageDataObservable.subscribe([this](std::tuple<unsigned char*, int, int> dataImg) {

		SendImageScreen(std::get<0>(dataImg), std::get<1>(dataImg), std::get<2>(dataImg));
		});

	subscriptionImageData.clear();
}

void FlowVideo::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
		});

	auto subscriptionError = observerError.subscribe([this](Either* either) {

		shootError.on_next(either);
		});

	auto observerErrorTracking = tracking->observableError.map([](Either* either) {
		return either;
	});


	auto subscriptionErrorTracking = observerErrorTracking.subscribe([this](Either* either) {		
		shootError.on_next(either);
	});

	/*auto observerDatabase = backRest->observableUserJSON.map([](string jsonUser) {
		return jsonUser;
	});

	auto subscriptionDatabase = observerDatabase.subscribe([this](string jsonUser) {
		
		shootUserJSON.on_next(jsonUser);
		
	});

	subscriptionDatabase.clear();*/
}

void FlowVideo::SendImageScreen(unsigned char* data, int size, int index) {
	if (index == indexFrame)
	{		
		std::vector<unsigned char> bufferMap(data,
			data + size);
		std::thread trt(&FlowVideo::RunTracking, this, bufferMap, index);
		trt.detach();

		clock_t timeStart1 = clock();
		frameView->SetImageData(indexFrame, data, size);
		DrawRectangles();
		clock_t duration1 = clock() - timeStart1;
		int durationMs1 = int(1000 * ((float)duration1) / CLOCKS_PER_SEC);
		//printf("   LOAD IMAGE AND DRAW RECTANGLE FOR OPENGL time: %d index: %d\n", durationMs1, indexFrame);
		if (!flagRate) {
			flagRate = true;
			int fr = (1000 / tracking->GetSequenceFps()) - durationMs1;
			frameView->SetElapse(indexFrame, fr);
			printf("   FPS: %d TIME: %d INDEX: %d\n", tracking->GetSequenceFps(), durationMs1, indexFrame);
		}
		bufferMap.clear();
	}
	
}


void FlowVideo::DrawRectangles() {
	int countRectangles = 0;
	float* coordinatesFace = tracking->GetCoordiantesRectangle();
	float* colorLine = tracking->GetColorRectangle();
	for (int i = 0; i < COORDINATES_X_ALL_IMAGES; i += 4)
	{
		if (coordinatesFace[i] != 0) {
			
			float x = coordinatesFace[i];
			float y = coordinatesFace[i + 1];
			float w = coordinatesFace[i + 2];
			float h = coordinatesFace[i + 3];
			float c = colorLine[countRectangles];

			float xc = x + (w / 2);
			float yc = y + (h / 2);
			float rx = w / ((frameView->GetWidthImage(indexFrame) == 0) ? 1.0f : (float)frameView->GetWidthImage(indexFrame));
			float ry = h / ((frameView->GetHeightImage(indexFrame) == 0) ? 1.0f : (float)frameView->GetHeightImage(indexFrame));
			//float rx = w / (float)frameView->GetWidthImage(indexFrame);
			//float ry = h / (float)frameView->GetHeightImage(indexFrame);
			frameView->SetRatioHeight(indexFrame, countRectangles, ry);
			frameView->SetRatioWidth(indexFrame, countRectangles, rx);
			frameView->SetPositionX(indexFrame, countRectangles, xc);
			frameView->SetPositionY(indexFrame, countRectangles, yc);
			frameView->SetColourR(indexFrame, countRectangles, c);
			frameView->SetColourG(indexFrame, countRectangles, c);					
			
		}
		else
		{
			frameView->SetRatioHeight(indexFrame, countRectangles, 0.0f);
			frameView->SetRatioWidth(indexFrame, countRectangles, 0.0f);
			frameView->SetPositionX(indexFrame, countRectangles, 0.0f);
			frameView->SetPositionY(indexFrame, countRectangles, 0.0f);
			frameView->SetColourR(indexFrame, countRectangles, 1.0f);
			frameView->SetColourG(indexFrame, countRectangles, 1.0f);
		}

		countRectangles++;
	}

}



void FlowVideo::RunTracking(std::vector<unsigned char> buffer, int index) {
	if (!tracking->GetFlagTracking() && index == indexFrame)
	{
		//flagTracking = true;
		tracking->SetFlagTracking(true);
		tracking->FaceTracking(buffer);		
		//FaceTracking(buffer);
	}
}


GstFlowReturn FlowVideo::CaptureGstBuffer(GstAppSink *appsink, gpointer /*data*/)
{	
	//clock_t timeStart1 = clock();
	int maxBuffer = gst_app_sink_get_max_buffers(appsink);
	
	GstSample *sample = gst_app_sink_pull_sample(appsink);
	if (sample == NULL) {
		error->CheckError(1201, error->less, "GStreamer: gst_app_sink_pull_sample");
		return GST_FLOW_ERROR;
	}
	
	GstCaps* caps = gst_sample_get_caps(sample);
	GstBuffer* buffer = gst_sample_get_buffer(sample);
	GstStructure* structure = gst_caps_get_structure(caps, 0);
	
	//const GstStructure* info = gst_sample_get_info(sample);
	
	int width = g_value_get_int(gst_structure_get_value(structure, "width"));
	int height = g_value_get_int(gst_structure_get_value(structure, "height"));
	int numerator = gst_value_get_fraction_numerator(gst_structure_get_value(structure, "framerate"));
	int denominator = gst_value_get_fraction_denominator(gst_structure_get_value(structure, "framerate"));
	
	int frameRate = numerator / ((denominator == 0) ? 1 : denominator);
	//g_print("caps: %s\n", gst_caps_to_string(caps));
	auto tupleInit = std::make_tuple(width, height, frameRate, maxBuffer);
	shootInitFrame.on_next(tupleInit);	

	/*if (info != NULL)
	{
		g_print("caps: %s\n", gst_structure_to_string(info));
	}*/
		

	GstMapInfo map_info;

	if (!gst_buffer_map((buffer), &map_info, GST_MAP_READ)) {
		gst_buffer_unmap((buffer), &map_info);
		gst_sample_unref(sample);
		error->CheckError(1202, error->less, "GStreamer: gst_buffer_map");
		return GST_FLOW_ERROR;
	}

	if (map_info.data != NULL) {
		
		auto tupleImageData = std::make_tuple(map_info.data, (int)map_info.size, maxBuffer);
		shootImageData.on_next(tupleImageData);
		
		
	}

	
	gst_buffer_unmap(buffer, &map_info);
	gst_sample_unref(sample);	
	/*clock_t duration1 = clock() - timeStart1;
	int durationMs1 = int(1000 * ((float)duration1) / CLOCKS_PER_SEC);
	printf("   CAPTURE GST BUFFER time: %d\n", durationMs1);*/
	return GST_FLOW_OK;
}

gboolean FlowVideo::MessageCallback(GstBus *bus, GstMessage *message, gpointer data)
{

	switch (GST_MESSAGE_TYPE(message)) {
	case GST_MESSAGE_ERROR: {
		GError *err;
		gchar *debug;

		gst_message_parse_error(message, &err, &debug);
		g_print("Error: %s\n", err->message);
		error->CheckError(1203, error->less, err->message);
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
		//cout << "DEFAULT" << endl;
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

void FlowVideo::RunFlowVideo() {
	
	
	tracking->InitITracking();
	
	
	gchar *descr = DescriptionFlow(configuration->GetSourceFlow());
	
	
	gst_init(NULL, NULL);
	
	GError *gError = nullptr;	
	pipeline = gst_parse_launch(descr, &gError);
	if (gError) {
		g_print("could not construct pipeline: %s\n", gError->message);
		error->CheckError(1204, error->less, gError->message);
		g_error_free(gError);
		exit(-1);
	}

	GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");

	gst_app_sink_set_emit_signals((GstAppSink*)sink, true);
	gst_app_sink_set_drop((GstAppSink*)sink, true);
	gst_app_sink_set_max_buffers((GstAppSink*)sink, indexFrame);
		
	GstAppSinkCallbacks* appsink_callbacks = (GstAppSinkCallbacks*)malloc(sizeof(GstAppSinkCallbacks));
	appsink_callbacks->eos = NULL;
	appsink_callbacks->new_preroll = NULL;
	appsink_callbacks->new_sample = &FlowVideo::CaptureGstBuffer;
	gst_app_sink_set_callbacks(GST_APP_SINK(sink), appsink_callbacks, (gpointer)NULL, free);
	
	
	GstBus *bus;
	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	gst_bus_add_watch(bus, &FlowVideo::MessageCallback, nullptr);
	gst_object_unref(bus);
	
	GstStateChangeReturn ret;
	ret = gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE) {
		g_printerr("Unable to set the pipeline to the playing state.\n");
		error->CheckError(1205, error->less, "GStreamer: Unable to set the pipeline to the playing state.\n");
		gst_object_unref(pipeline);
		exit(-1);
	}
	
	loop = g_main_loop_new(NULL, false);
	g_main_loop_run(loop);
		

	gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
	gst_object_unref(GST_OBJECT(pipeline));
	
	tracking->ClearAllCoordinatesImage();
	
	tracking->SetCountFrameTracking(0);
	
	tracking->TerminateTracking();
	
}


gchar* FlowVideo::DescriptionFlow(int optionFlow) {
	gchar *descr = nullptr;

	//"! video/x-raw, width=(int)%d, height=(int)%d, format=(string)I420, framerate=30/1 "
	switch (optionFlow) {
	case 1: // IP CAMERA
		descr = g_strdup_printf(
			"rtspsrc location=%s "
			"! application/x-rtp, payload=96 ! rtph264depay ! h264parse ! avdec_h264 "
			"! decodebin ! videoconvert "
			"! video/x-raw, format=(string)I420 "
			"! jpegenc quality=100 "
			"! appsink name=sink emit-signals=true sync=false max-buffers=1 drop=true",
			configuration->GetIpCamera().c_str()
		);
		
		break;
	case 2: // FILE		
		descr = g_strdup_printf(
			"filesrc location=%s "
			"! decodebin ! videoconvert "			
			"! video/x-raw, format=(string)I420 "
			"! jpegenc quality=100 "
			"! appsink name=sink emit-signals=true sync=true max-buffers=1 drop=true",
			configuration->GetFileVideo().c_str()
		);
		
		break;
	case 3: // CAMERA   
		descr = g_strdup_printf(
			"v4l2src device=%s "
			"! decodebin ! videoconvert "			
			"! video/x-raw, format=(string)I420  "
			"! jpegenc "
			"! appsink name=sink emit-signals=true sync=true max-buffers=1 drop=true",
			configuration->GetDeviceVideo().c_str()
		);		
		break;
	}
	return descr;

}

//void FlowVideo::ShowWindow(int option) {
//	GstState cur_state;
//	
//	gst_element_get_state(GST_ELEMENT(pipeline), &cur_state, NULL, 0);
//	if (cur_state == GST_STATE_PLAYING) {
//		std::wstring stemp = std::wstring(nameWindow.begin(), nameWindow.end());
//		LPCWSTR sw = stemp.c_str();
//		HWND win_handle = FindWindow(0, sw);
//		switch (option)
//		{
//		case 1:
//			SetWindowPos(win_handle, HWND_BOTTOM, 0, 0, 0, 0, wFlags);
//			break;
//		case -1:
//			SetWindowPos(win_handle, HWND_TOPMOST, 0, 0, 0, 0, wFlags);
//			break;
//		}
//	}
//		
//}
//
//void FlowVideo::ResetLowScore() {
//	backRest->ResetLowScore();
//}
//
//int FlowVideo::GetCountLowScore() {
//	return backRest->GetCountLowScore();
//}
//
//void FlowVideo::ResetCountNotDetect() {
//	backRest->ResetCountNotDetect();
//}
//
//int FlowVideo::GetCountNotDetect() {
//	return backRest->GetCountNotDetect();
//}
//
//void FlowVideo::SetDeepTrack(string value) {
//	deepTrack = value;
//}
//
//void FlowVideo::ResetCountRepeatUser() {
//	backRest->ResetCountRepeatUser();
//}
//
//int FlowVideo::GetCountRepeatUser() {
//	return backRest->GetCountRepeatUser();
//}

void FlowVideo::StatePlay() {
	GstState cur_state;
	if (pipeline != NULL)
	{
		gst_element_get_state(GST_ELEMENT(pipeline), &cur_state, NULL, 0);
		if (cur_state == GST_STATE_PAUSED) {
			gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
		}		
	}
	
}

void FlowVideo::StatePaused() {	
	GstState cur_state;
	if (pipeline != NULL)
	{
		gst_element_get_state(GST_ELEMENT(pipeline), &cur_state, NULL, 0);
		if (cur_state == GST_STATE_PLAYING) {
			gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PAUSED);
		}
	}
	
}

void FlowVideo::SetFinishLoop() {
	
	GstState cur_state;
	if (pipeline != NULL)
	{
		gst_element_get_state(GST_ELEMENT(pipeline), &cur_state, NULL, 0);
		if (cur_state == GST_STATE_PLAYING) {
			gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PAUSED);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));			
		}
		
		g_main_loop_quit(loop);
	}
	
			
}