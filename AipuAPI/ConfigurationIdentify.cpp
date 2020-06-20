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

		if (it.key().asString() == AMINEYE) {
			minEyeDistance = it->asInt();
		}
		else if (it.key().asString() == AMAXEYE) {
			maxEyeDistance = it->asInt();
		}
		else if (it.key().asString() == IDENTIFICATIONSPEED) {
			identificationSpeed = it->asInt();
		}
		else if (it.key().asString() == FACEDETECTIONFORCED) {
			faceDetectionForced = it->asInt();
		}
		else if (it.key().asString() == SIMILARITYTHRESHOLD) {
			similarityThreshold = it->asInt();
		}
		else if (it.key().asString() == FACEDETECTIONTHRESHOLD) {
			faceDetectionThreshold = it->asInt();
		}
		else if (it.key().asString() == BESTMATCHEDCANDIDATES) {
			bestMatchedCandidates = it->asInt();
		}
		else if (it.key().asString() == ISREGISTER) {
			isRegister = it->asInt();
		}
	}

}

void ConfigurationIdentify::ParseJSONToObject() {
	configuration->jsonBody.clear();
	configuration->stringJSON = configuration->manageFile->ReadFileText();
	bool parsingSuccessful = configuration->reader.parse(configuration->stringJSON, configuration->jsonBody);
	if (parsingSuccessful)
	{
		SetValueJSONToConfiguration();
	}
	else {
		error->CheckError(ERROR_PARSING, error->medium);
	}

}

void ConfigurationIdentify::ParseMapToJSON() {
	configuration->jsonBody.clear();
	configuration->jsonParams.clear();
	std::map<std::string, std::int16_t> params;

	params.insert(std::pair<std::string, std::int16_t>(AMINEYE, minEyeDistance));
	params.insert(std::pair<std::string, std::int16_t>(AMAXEYE, maxEyeDistance));
	params.insert(std::pair<std::string, std::int16_t>(IDENTIFICATIONSPEED, identificationSpeed));
	params.insert(std::pair<std::string, std::int16_t>(FACEDETECTIONFORCED, faceDetectionForced));
	params.insert(std::pair<std::string, std::int16_t>(SIMILARITYTHRESHOLD, similarityThreshold));
	params.insert(std::pair<std::string, std::int16_t>(FACEDETECTIONTHRESHOLD, faceDetectionThreshold));
	params.insert(std::pair<std::string, std::int16_t>(BESTMATCHEDCANDIDATES, bestMatchedCandidates));
	params.insert(std::pair<std::string, std::int16_t>(ISREGISTER, isRegister));
	std::map<std::string, std::int16_t>::const_iterator it = params.begin(),
		end = params.end();

	for (; it != end; ++it) {
		configuration->jsonParams[it->first] = it->second;

	}

	configuration->jsonBody[CONFIGURATION] = IDENTIFY_CONFIGURATION;
	configuration->jsonBody[PARAMS] = configuration->jsonParams;

	configuration->SaveConfiguration();

}

string ConfigurationIdentify::ParseMapToJSONForIdentify() {
	configuration->jsonBody.clear();
	std::map<std::string, std::int16_t> params;
	params.insert(std::pair<std::string, std::int16_t>(IDENTIFICATIONSPEED, identificationSpeed));
	params.insert(std::pair<std::string, std::int16_t>(AMINEYE, minEyeDistance));
	params.insert(std::pair<std::string, std::int16_t>(AMAXEYE, maxEyeDistance));
	params.insert(std::pair<std::string, std::int16_t>(FACEDETECTIONFORCED, faceDetectionForced));
	params.insert(std::pair<std::string, std::int16_t>(SIMILARITYTHRESHOLD, similarityThreshold));
	params.insert(std::pair<std::string, std::int16_t>(FACEDETECTIONTHRESHOLD, faceDetectionThreshold));
	params.insert(std::pair<std::string, std::int16_t>(BESTMATCHEDCANDIDATES, bestMatchedCandidates));
	std::map<std::string, std::int16_t>::const_iterator it = params.begin(),
		end = params.end();

	for (; it != end; ++it) {
		configuration->jsonBody[it->first] = it->second;

	}

	configuration->WriteJSONToString();

	return configuration->stringJSON;
}