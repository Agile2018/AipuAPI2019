#include "ConfigurationFlowVideo.h"

ConfigurationFlowVideo::ConfigurationFlowVideo()
{
}

ConfigurationFlowVideo::~ConfigurationFlowVideo()
{
}

void ConfigurationFlowVideo::SetValueJSONToConfiguration() {
	const Json::Value params = configuration->jsonBody[PARAMS];
	for (Json::Value::const_iterator it = params.begin();
		it != params.end(); ++it) {

		if (it.key().asString() == SOURCEFLOW) {
			sourceFlow = it->asInt();
		}
		else if (it.key().asString() == IPCAMERA) {
			if (!it->asString().empty()) {
				ipCamera = it->asString();
			}			
		}
		else if (it.key().asString() == FILEVIDEO) {

			if (!it->asString().empty()) {
				fileVideo = it->asString();
			}
		}
		else if (it.key().asString() == DEVICEVIDEO) {
			if (!it->asString().empty()) {
				deviceVideo = it->asString();
			}
		}
		else if (it.key().asString() == VIDEOSCALEMETHOD) {
			videoScaleMethod = it->asInt();
		}
		
	}

}

void ConfigurationFlowVideo::ParseJSONToObject() {
	configuration->jsonBody.clear();
	configuration->stringJSON = configuration->manageFile->ReadFileText();
	bool parsingSuccessful = configuration->reader.parse(configuration->stringJSON, configuration->jsonBody);
	if (parsingSuccessful)
	{
		SetValueJSONToConfiguration();
	}

}

void ConfigurationFlowVideo::ParseMapToJSON() {
	string sf = to_string(sourceFlow);
	string vs = to_string(videoScaleMethod);
	configuration->jsonBody.clear();
	configuration->jsonParams.clear();
	std::map<std::string, std::string> params;

	params.insert(std::pair<std::string, std::string>(SOURCEFLOW, sf));
	params.insert(std::pair<std::string, std::string>(IPCAMERA, ipCamera));
	params.insert(std::pair<std::string, std::string>(FILEVIDEO, fileVideo));
	params.insert(std::pair<std::string, std::string>(DEVICEVIDEO, deviceVideo));
	params.insert(std::pair<std::string, std::string>(VIDEOSCALEMETHOD, vs));
	std::map<std::string, std::string>::const_iterator it = params.begin(),
		end = params.end();

	for (; it != end; ++it) {
		configuration->jsonParams[it->first] = it->second;

	}

	configuration->jsonBody[CONFIGURATION] = FLOWVIDEO_CONFIGURATION;
	configuration->jsonBody[PARAMS] = configuration->jsonParams;

	//SaveConfiguration();

}