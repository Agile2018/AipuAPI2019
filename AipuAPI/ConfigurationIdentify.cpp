#include "ConfigurationIdentify.h"

ConfigurationIdentify::ConfigurationIdentify()
{
	ObserverError();
}

ConfigurationIdentify::~ConfigurationIdentify()
{
}

void ConfigurationIdentify::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
		shootError.on_next(either);
	});
}

void ConfigurationIdentify::SetValueJSONToConfiguration() {
	const Json::Value params = configuration->jsonBody[PARAMS];
	for (Json::Value::const_iterator it = params.begin();
		it != params.end(); ++it) {

		if (it.key().asString() == CFG_BEST_CANDIDATES_COUNT) {
			bestMatchedCandidates = it->asInt();
		}
		else if (it.key().asString() == CFG_SIMILARITY_THRESHOLD) {
			similarityThreshold = it->asInt();
			identificationThreshold = it->asInt();
		}
		else if (it.key().asString() == CFG_IDENTIFICATION_SPEED) {
			identificationSpeed = it->asInt();
		}
		else if (it.key().asString() == CFG_IFACE_DETECT_FORCED) {
			faceDetectionForced = it->asInt();
		}
		else if (it.key().asString() == CFG_IFACE_IGNORE_MULTIPLE_FACES) {
			ignoreMultipleFaces = it->asInt();
		}
		else if (it.key().asString() == CFG_IFACE_DETECTION_MODE) {
			faceDetectionMode = it->asInt();
		}
		else if (it.key().asString() == CFG_IFACE_EXTRACTION_MODE) {
			faceExtractionMode = it->asInt();
		}
		else if (it.key().asString() == CFG_IFACE_DETECTION_THRESHOLD) {
			detectionThreshold = it->asInt();
		}
		else if (it.key().asString() == AFACE_PARAMETER_SCORE_MIN) {
			score_min = it->asInt();
		}
		else if (it.key().asString() == AFACE_PARAMETER_SCORE_MAX) {
			score_max = it->asInt();
		}
		else if (it.key().asString() == AFACE_PARAMETER_ENROLL) {
			isEnroll = it->asInt();
		}
		else if (it.key().asString() == CFG_SIMILARITY_THRESHOLD_DEDUPLICATION) {
			similarityThresholdDeduplication = it->asInt();
		}
		else if (it.key().asString() == AFACE_PARAMETER_DEDUPLICATION) {
			isDeduplication = it->asInt();
		}
		else if (it.key().asString() == AFACE_PARAMETER_MAXIMUM_TEMPLATES) {
			maximumConcatenateTemplates = it->asInt();
		}
		else if (it.key().asString() == AFACE_PARAMETER_CONCATENATE_TEMPLATES) {
			isConcatenateTemplates = it->asInt();
		}
		else if (it.key().asString() == AFACE_PARAMETER_CONCATENATION_MODE) {
			concatenateMode = it->asInt();
		}
		else if (it.key().asString() == AFACE_PARAMETER_VERIFICATION_SCORE) {
			verificationScore = it->asInt();
		}
	}

}

void ConfigurationIdentify::ParseJSONToObject() {
	configuration->jsonBody.clear();
	//configuration->stringJSON = configuration->manageFile->ReadFileText();
	bool parsingSuccessful = configuration->reader.parse(configuration->stringJSON, configuration->jsonBody);
	if (parsingSuccessful)
	{
		SetValueJSONToConfiguration();
	}
	else {
		error->CheckError(ERROR_PARSING, error->medium);
	}
	configuration->stringJSON = configuration->stringJSON.empty();
}

//void ConfigurationIdentify::ParseMapToJSON() {
//	configuration->jsonBody.clear();
//	configuration->jsonParams.clear();
//	std::map<std::string, std::int16_t> params;
//
//	params.insert(std::pair<std::string, std::int16_t>(AMINEYE, minEyeDistance));
//	params.insert(std::pair<std::string, std::int16_t>(AMAXEYE, maxEyeDistance));
//	params.insert(std::pair<std::string, std::int16_t>(IDENTIFICATIONSPEED, identificationSpeed));
//	params.insert(std::pair<std::string, std::int16_t>(FACEDETECTIONFORCED, faceDetectionForced));
//	params.insert(std::pair<std::string, std::int16_t>(SIMILARITYTHRESHOLD, similarityThreshold));
//	params.insert(std::pair<std::string, std::int16_t>(BIOMETRICLOGLEVEL, biometricLogLevel));
//	params.insert(std::pair<std::string, std::int16_t>(BESTMATCHEDCANDIDATES, bestMatchedCandidates));
//	params.insert(std::pair<std::string, std::int16_t>(IGNOREMULTIPLEFACES, ignoreMultipleFaces));
//	params.insert(std::pair<std::string, std::int16_t>(FACEDETECTIONMODE, faceDetectionMode));
//	params.insert(std::pair<std::string, std::int16_t>(SEARCHEXTRACTIONTHREADS, searchExtractionThreads));
//	params.insert(std::pair<std::string, std::int16_t>(FACEEXTRACTIONMODE, faceExtractionMode));	
//	params.insert(std::pair<std::string, std::int16_t>(ISREGISTER, isRegister));
//	std::map<std::string, std::int16_t>::const_iterator it = params.begin(),
//		end = params.end();
//
//	for (; it != end; ++it) {
//		configuration->jsonParams[it->first] = it->second;
//
//	}
//
//	configuration->jsonBody[CONFIGURATION] = IDENTIFY_CONFIGURATION;
//	configuration->jsonBody[PARAMS] = configuration->jsonParams;
//
//	configuration->SaveConfiguration();
//
//}
//
//string ConfigurationIdentify::ParseMapToJSONForIdentify() {
//	configuration->jsonBody.clear();
//	std::map<std::string, std::int16_t> params;
//	params.insert(std::pair<std::string, std::int16_t>(IDENTIFICATIONSPEED, identificationSpeed));
//	params.insert(std::pair<std::string, std::int16_t>(AMINEYE, minEyeDistance));
//	params.insert(std::pair<std::string, std::int16_t>(AMAXEYE, maxEyeDistance));
//	params.insert(std::pair<std::string, std::int16_t>(FACEDETECTIONFORCED, faceDetectionForced));
//	params.insert(std::pair<std::string, std::int16_t>(SIMILARITYTHRESHOLD, similarityThreshold));
//	params.insert(std::pair<std::string, std::int16_t>(BIOMETRICLOGLEVEL, biometricLogLevel));
//	params.insert(std::pair<std::string, std::int16_t>(BESTMATCHEDCANDIDATES, bestMatchedCandidates));
//	params.insert(std::pair<std::string, std::int16_t>(IGNOREMULTIPLEFACES, ignoreMultipleFaces));
//	params.insert(std::pair<std::string, std::int16_t>(FACEDETECTIONMODE, faceDetectionMode));
//	params.insert(std::pair<std::string, std::int16_t>(SEARCHEXTRACTIONTHREADS, searchExtractionThreads));
//	params.insert(std::pair<std::string, std::int16_t>(FACEEXTRACTIONMODE, faceExtractionMode));
//
//	std::map<std::string, std::int16_t>::const_iterator it = params.begin(),
//		end = params.end();
//
//	for (; it != end; ++it) {
//		configuration->jsonBody[it->first] = it->second;
//
//	}
//
//	configuration->WriteJSONToString();
//
//	return configuration->stringJSON;
//}