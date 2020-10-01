#include "ConfigurationGlobal.h"

ConfigurationGlobal::ConfigurationGlobal()
{
	ObserverError();
}

ConfigurationGlobal::~ConfigurationGlobal()
{
}

void ConfigurationGlobal::ParseJSONToObject() {
	configuration->jsonBody.clear();
	configuration->stringJSON = configuration->manageFile->ReadFileText();
	bool parsingSuccessful = configuration->reader.parse(configuration->stringJSON, configuration->jsonBody);
	if (parsingSuccessful)
	{
		SetValueJSONToConfiguration();
	}
	else {
		error->CheckError(ERROR_PARSING, error->medium, LABEL_ERROR_PARSING);
	}

}

void ConfigurationGlobal::SetValueJSONToConfiguration() {
	const Json::Value params = configuration->jsonBody[PARAMS];
	for (Json::Value::const_iterator it = params.begin();
		it != params.end(); ++it) {

		if (it.key().asString() == GLOBAL_MIN_VALID_IMAGE_SIZE) {
			minValidImageSize = it->asInt();
		}
		else if (it.key().asString() == GLOBAL_GPU_DEVICE_ID) {
			if (!it->asString().empty()) {
				gpuDeviceId = it->asString();
			}
		}
		else if (it.key().asString() == GLOBAL_GPU_ENABLED) {

			if (!it->asString().empty()) {
				gpuEnabled = it->asString();
			}
		}
		else if (it.key().asString() == GLOBAL_THREAD_NUM) {
			if (!it->asString().empty()) {
				threadNum = it->asString();
			}
		}
		else if (it.key().asString() == GLOBAL_THREAD_MANAGEMENT_MODE) {
			threadManagementMode = it->asInt();
		}
		else if (it.key().asString() == GLOBAL_CFG_LOG_LEVEL) {
			logLevel = it->asInt();
		}
		else if (it.key().asString() == AFACE_PARAMETER_GPU_ENABLED) {
			ourEnabledGPU = it->asInt();
		}
	}

}

void ConfigurationGlobal::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
		});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
		shootError.on_next(either);
		});
}