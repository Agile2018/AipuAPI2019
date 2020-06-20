#ifndef ConfigurationTracking_h
#define ConfigurationTracking_h

#include "Configuration.h"

class ConfigurationTracking
{
public:
	ConfigurationTracking();
	~ConfigurationTracking();
	void ParseJSONToObject();
	void ParseMapToJSON();

	void SetMinEyeDistance(int value) {
		minEyeDistance = value;
	}

	int GetMinEyeDistance() {
		return minEyeDistance;
	}

	void SetMaxEyeDistance(int value) {
		maxEyeDistance = value;
	}

	int GetMaxEyeDistance() {
		return maxEyeDistance;
	}

	void SetFaceConfidenceThresh(int value) {
		faceConfidenceThresh = value;
	}

	int GetFaceConfidenceThresh() {
		return faceConfidenceThresh;
	}

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

	void SetRefreshInterval(int value) {
		refreshInterval = value;
	}

	int GetRefreshInterval() {
		return refreshInterval;
	}

	void SetDeepTrack(int value) {
		deepTrack = value;
	}

	int GetDeepTrack() {
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

	void SetNameFileConfiguration(string name) {
		configuration->SetNameFileConfiguration(name);
	}
	void SetNameDirectory(string name) {
		configuration->SetNameDirectory(name);
	}

	void SetQualityModel(int value) {
		qualityModel = value;
	}

	int GetQualityModel() {
		return qualityModel;
	}

private:
	int minEyeDistance = 20;          // minimal eye distance in input image
	int maxEyeDistance = 200;         // maximal eye distance in input image
	int faceConfidenceThresh = 450;         // face detection confidence threshold		
	int trackingMode = 1;
	int trackSpeed = 2;
	int motionOptimization = 2;
	int deepTrack = 1;
	int sequenceFps = 30;                   // fps of video	
	int timeDeltaMs = 1000 / sequenceFps;
	int refreshInterval = 2000;
	int maxFaces = 5;
	int qualityModel = 60;
	const string PARAMS = "Params";
	const string MAXFACES = "maxfaces";
	const string MINEYE = "mineye";
	const string MAXEYE = "maxeye";
	const string FACE_DISCOVERY_FREQUENCE_MS = "refreshInterval";
	const string FACEDET_CONFIDENCE_THRESHOLD = "faceConfidenceThresh";
	const string TRACK_TRACKING_MODE = "trackingMode";
	const string TRACK_SPEED_ACCURACY_MODE = "trackSpeed";
	const string TRACK_MOTION_OPTIMIZATION = "motionOptimization";
	const string TRACK_DEEP_TRACK = "deepTrack";
	const string QUALITYMODEL = "qualitymodel";
	const string CONFIGURATION = "configuration";
	const string TRACKING_CONFIGURATION = "tracking_configuration";
	Configuration* configuration = new Configuration();
	void SetValueJSONToConfiguration();
	
};


#endif // !ConfigurationTracking_h

