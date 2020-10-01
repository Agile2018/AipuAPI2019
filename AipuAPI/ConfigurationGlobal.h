#ifndef ConfigurationGlobal_h
#define ConfigurationGlobal_h

#include "ErrorAipuLib.h"
#include "Configuration.h"


class ConfigurationGlobal
{
public:
	ConfigurationGlobal();
	~ConfigurationGlobal();
	void ParseJSONToObject();

	void SetNameFileConfiguration(string name) {
		configuration->SetNameFileConfiguration(name);
	}
	void SetNameDirectory(string name) {
		configuration->SetNameDirectory(name);
	}

	int GetMinValidImageSize() {
		return minValidImageSize;
	}

	string GetGpuDeviceId() {
		return gpuDeviceId;
	}

	string GetGpuEnabled() {
		return gpuEnabled;
	}

	int GetThreadManagementMode() {
		return threadManagementMode;
	}

	string GetThreadNum() {
		return threadNum;
	}

	int GetLogLevel() {
		return logLevel;
	}

	int GetOurEnabledGPU() {
		return ourEnabledGPU;
	}

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();

private:
	ErrorAipuLib* error = new ErrorAipuLib();
	Configuration* configuration = new Configuration();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	const string LABEL_ERROR_PARSING = "Error parsing the JSON string";

	const string GLOBAL_MIN_VALID_IMAGE_SIZE = "GLOBAL_MIN_VALID_IMAGE_SIZE";
	const string GLOBAL_GPU_DEVICE_ID = "GLOBAL_GPU_DEVICE_ID";
	const string GLOBAL_GPU_ENABLED = "GLOBAL_GPU_ENABLED";
	const string GLOBAL_THREAD_MANAGEMENT_MODE = "GLOBAL_THREAD_MANAGEMENT_MODE";
	const string GLOBAL_THREAD_NUM = "GLOBAL_THREAD_NUM";
	const string GLOBAL_CFG_LOG_LEVEL = "GLOBAL_CFG_LOG_LEVEL";
	const string AFACE_PARAMETER_GPU_ENABLED = "AFACE_PARAMETER_GPU_ENABLED";
	const string PARAMS = "paramsGlobal";
	const string CONFIGURATION = "configurationGlobal";
	const string DATABASE_CONFIGURATION = "Global processing";

	int minValidImageSize = 200;
	string gpuDeviceId = "0";
	string gpuEnabled = "true";
	int threadManagementMode = 1;
	string threadNum = "4";
	int logLevel = 0;
	int ourEnabledGPU = 1;
	void ObserverError();
	void SetValueJSONToConfiguration();
};


#endif // !ConfigurationGlobal_h

