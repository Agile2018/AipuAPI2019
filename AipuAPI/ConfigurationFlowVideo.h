#ifndef ConfigurationFlowVideo_h
#define ConfigurationFlowVideo_h

#include "Configuration.h"
#include "ErrorAipuLib.h"

class ConfigurationFlowVideo 
{
public:
	ConfigurationFlowVideo();
	~ConfigurationFlowVideo();
	void ParseJSONToObject();
	void ParseMapToJSON();

	int GetSourceFlow() {
		return sourceFlow;
	}

	string GetIpCamera() {
		return ipCamera;
	}

	string GetFileVideo() {
		return fileVideo;
	}

	string GetDeviceVideo() {
		return deviceVideo;
	}

	int GetVideoScaleMethod() {
		return videoScaleMethod;
	}

	void SetNameFileConfiguration(string name) {
		configuration->SetNameFileConfiguration(name);
	}
	void SetNameDirectory(string name) {
		configuration->SetNameDirectory(name);
	}

	string GetStringJSON() {
		return configuration->GetStringJSON();
	}

	void SetStringJSON(string jsonString) {
		configuration->SetStringJSON(jsonString);
	}

private:
	int sourceFlow = 1;
	string ipCamera = "";
	string fileVideo = "";
	string deviceVideo = "/dev/video0";	
	int videoScaleMethod = 1; //3 1

	const string PARAMS = "paramsFlow";
	const string SOURCEFLOW = "sourceFlow";
	const string IPCAMERA = "ipCamera";
	const string FILEVIDEO = "fileVideo";
	const string DEVICEVIDEO = "deviceVideo";
	const string VIDEOSCALEMETHOD = "videoScaleMethod";	
	const string CONFIGURATION = "configurationFlowVideo";
	const string FLOWVIDEO_CONFIGURATION = "Flow video";
	Configuration* configuration = new Configuration();
	void SetValueJSONToConfiguration();
};


#endif // !ConfigurationFlowVideo_h

