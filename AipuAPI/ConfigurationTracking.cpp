#include "ConfigurationTracking.h"

ConfigurationTracking::ConfigurationTracking()
{
}

ConfigurationTracking::~ConfigurationTracking()
{
}

void ConfigurationTracking::SetValueJSONToConfiguration() {
	const Json::Value params = configuration->jsonBody[PARAMS];
	for (Json::Value::const_iterator it = params.begin();
		it != params.end(); ++it) {

		if (it.key().asString() == MAXFACES) {
			maxFaces = it->asInt();
		}
		else if (it.key().asString() == MINEYE) {
			minEyeDistance = it->asInt();
		}
		else if (it.key().asString() == MAXEYE) {
			maxEyeDistance = it->asInt();
		}
		else if (it.key().asString() == FACE_DISCOVERY_FREQUENCE_MS) {
			refreshInterval = it->asInt();
		}
		else if (it.key().asString() == FACEDET_CONFIDENCE_THRESHOLD) {
			faceConfidenceThresh = it->asInt();
		}
		else if (it.key().asString() == TRACK_TRACKING_MODE) {
			trackingMode = it->asInt();
		}

		else if (it.key().asString() == TRACK_SPEED_ACCURACY_MODE) {
			trackSpeed = it->asInt();
		}
		else if (it.key().asString() == TRACK_MOTION_OPTIMIZATION) {
			motionOptimization = it->asInt();
		}
		else if (it.key().asString() == TRACK_DEEP_TRACK) {
			deepTrack = it->asInt();
		}		
		else if (it.key().asString() == QUALITYMODEL) {
			qualityModel = it->asInt();
		}
	}

}

void ConfigurationTracking::ParseJSONToObject() {
	configuration->jsonBody.clear();
	configuration->stringJSON = configuration->manageFile->ReadFileText();
	bool parsingSuccessful = configuration->reader.parse(configuration->stringJSON, configuration->jsonBody);
	if (parsingSuccessful)
	{
		SetValueJSONToConfiguration();
	}
	
}

void ConfigurationTracking::ParseMapToJSON() {
	configuration->jsonBody.clear();
	configuration->jsonParams.clear();
	std::map<std::string, std::int16_t> params;

	params.insert(std::pair<std::string, std::int16_t>(MAXFACES, maxFaces));
	params.insert(std::pair<std::string, std::int16_t>(MINEYE, minEyeDistance));
	params.insert(std::pair<std::string, std::int16_t>(MAXEYE, maxEyeDistance));
	params.insert(std::pair<std::string, std::int16_t>(FACE_DISCOVERY_FREQUENCE_MS, refreshInterval));
	params.insert(std::pair<std::string, std::int16_t>(FACEDET_CONFIDENCE_THRESHOLD, faceConfidenceThresh));
	params.insert(std::pair<std::string, std::int16_t>(TRACK_TRACKING_MODE, trackingMode));	
	params.insert(std::pair<std::string, std::int16_t>(TRACK_SPEED_ACCURACY_MODE, trackSpeed));
	params.insert(std::pair<std::string, std::int16_t>(TRACK_MOTION_OPTIMIZATION, motionOptimization));
	params.insert(std::pair<std::string, std::int16_t>(TRACK_DEEP_TRACK, deepTrack));
	params.insert(std::pair<std::string, std::int16_t>(QUALITYMODEL, qualityModel));
	std::map<std::string, std::int16_t>::const_iterator it = params.begin(),
		end = params.end();

	for (; it != end; ++it) {
		configuration->jsonParams[it->first] = it->second;

	}

	configuration->jsonBody[CONFIGURATION] = TRACKING_CONFIGURATION;
	configuration->jsonBody[PARAMS] = configuration->jsonParams;

	configuration->SaveConfiguration();

}