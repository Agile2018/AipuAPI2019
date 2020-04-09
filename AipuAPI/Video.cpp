#include "Video.h"

Video::Video()
{
}

Video::~Video()
{
}

void Video::Play(cv::Mat frame) {
	
	if (!init) {
		init = true;
		cv::namedWindow(nameWindow.c_str(), cv::WINDOW_NORMAL);		
	}

	if (!frame.empty()) {
		cv::imshow(nameWindow.c_str(), frame);
		cv::waitKey(1);
	}

	if (destroy)
	{
		videoIsDestroy = true;
		cv::destroyWindow(nameWindow.c_str());
	}
}