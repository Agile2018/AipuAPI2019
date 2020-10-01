#ifndef ConfigurationTracking_h
#define ConfigurationTracking_h

#include "Configuration.h"

class ConfigurationTracking
{
public:
	ConfigurationTracking();
	~ConfigurationTracking();
	void ParseJSONToObject();
	//void ParseMapToJSON();	

	void SetTrackingMode(int mode) {
		trackingMode = mode;
	}

	int GetTrackingMode() {
		return trackingMode;
	}

	void SetTrackSpeed(int speed) {
		trackSpeed = speed;
	}

	int GetTrackSpeed() {
		return trackSpeed;
	}

	void SetMotionOptimization(int motion) {
		motionOptimization = motion;
	}

	int GetMotionOptimization() {
		return motionOptimization;
	}

	void SetDiscoveryFrequenceMS(int value) {
		discoveryFrequenceMS = value;
	}

	int GetDiscoveryFrequenceMS() {
		return discoveryFrequenceMS;
	}

	void SetDeepTrack(string value) {
		deepTrack = value;
	}

	string GetDeepTrack() {
		return deepTrack;
	}

	void SetSequenceFps(int value) {
		if (value != 0)
		{
			sequenceFps = value;
			timeDeltaMs = 1000 / sequenceFps;
		}

	}

	int GetSequenceFps() {
		return sequenceFps;
	}

	int GetTimeDeltaMs() {
		return timeDeltaMs;
	}

	void SetMaxFaces(int value) {
		maxFaces = value;
	}

	int GetMaxFaces() {
		return maxFaces;
	}
	
	void SetStringJSON(string jsonString) {
		configuration->SetStringJSON(jsonString);
	}
	
	int GetCountRedetectTimeDelta() {
		return countRedetectTimeDelta;
	}

private:	
	int trackingMode = 2;
	int trackSpeed = 2;
	int motionOptimization = 2;
	int discoveryFrequenceMS = 2000;
	string deepTrack = "true";
	int countRedetectTimeDelta = 0;

	int sequenceFps = 30;                   // fps of video	
	int timeDeltaMs = 1000 / sequenceFps;	
	int maxFaces = 5;

	const string PARAMS = "Tracking processing";
	const string FACE_DISCOVERY_FREQUENCE_MS = "TRACK_FACE_DISCOVERY_FREQUENCE_MS";
	const string COUNT_REDETECT_TIME_DELTA = "COUNT_REDETECT_TIME_DELTA";
	const string TRACK_TRACKING_MODE = "TRACK_TRACKING_MODE";
	const string TRACK_SPEED_ACCURACY_MODE = "TRACK_SPEED_ACCURACY_MODE";
	const string TRACK_MOTION_OPTIMIZATION = "TRACK_MOTION_OPTIMIZATION";
	const string TRACK_DEEP_TRACK = "TRACK_DEEP_TRACK";
	const string CONFIGURATION = "configurationTrackingProcessing";
	const string TRACKING_CONFIGURATION = "Tracking processing";	
	Configuration* configuration = new Configuration();
	void SetValueJSONToConfiguration();
	
};


#endif // !ConfigurationTracking_h

