#include "ConfigurationIFace.h"

ConfigurationIFace::ConfigurationIFace()
{
	ObserverError();
}

ConfigurationIFace::~ConfigurationIFace()
{
}

void ConfigurationIFace::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
		shootError.on_next(either);
	});
}

void ConfigurationIFace::SetValueJSONToConfiguration() {
	const Json::Value params = configuration->jsonBody[PARAMS];
	//cout << params.toStyledString() << endl;
	for (Json::Value::const_iterator it = params.begin();
		it != params.end(); ++it) {

		if (it.key().asString() == MAXFACESDETECT) {
			maxfaceDetect = it->asInt();
		}
		else if (it.key().asString() == MINFACE) {
			minFaceSize = it->asFloat();
		}
		else if (it.key().asString() == MAXFACE) {
			maxFaceSize = it->asFloat();
		}
		else if (it.key().asString() == CONFIDENCE_THRESHOLD) {
			confidenceThreshold = it->asInt();
		}
		else if (it.key().asString() == SPEED_ACCURACY_MODE) {
			speedAccurancyMode = it->asInt();
		}
		else if (it.key().asString() == FACETMPLEXT_SPEED_ACCURACY_MODE) {
			faceTemplextspeedAccurancyMode = it->asInt();
		}
		else if (it.key().asString() == QUALITYMODEL) {
			qualityModel = it->asInt();
		}
		else if (it.key().asString() == FACE_CROP_IMAGE) {
			getFaceCropImage = it->asInt();
		}
	}

}

void ConfigurationIFace::ParseJSONToObject() {
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

//void ConfigurationIFace::ParseMapToJSON() {
//	configuration->jsonBody.clear();
//	configuration->jsonParams.clear();
//	std::map<std::string, std::int16_t> params;
//
//	params.insert(std::pair<std::string, std::int16_t>(MAXFACES, maxDetect));
//	params.insert(std::pair<std::string, std::int16_t>(MINEYE, minEyeDistance));
//	params.insert(std::pair<std::string, std::int16_t>(MAXEYE, maxEyeDistance));
//	params.insert(std::pair<std::string, std::int16_t>(PRECISION, accuracy));
//	params.insert(std::pair<std::string, std::int16_t>(MODEDETECT, modeDetect));
//	params.insert(std::pair<std::string, std::int16_t>(EXTRACTIONMODE, extractionMode));
//	params.insert(std::pair<std::string, std::int16_t>(QUALITYMODEL, qualityModel));
//	std::map<std::string, std::int16_t>::const_iterator it = params.begin(),
//		end = params.end();
//
//	for (; it != end; ++it) {
//		configuration->jsonParams[it->first] = it->second;
//
//	}
//
//	configuration->jsonBody[CONFIGURATION] = DETECT_CONFIGURATION;
//	configuration->jsonBody[PARAMS] = configuration->jsonParams;
//
//	configuration->SaveConfiguration();
//}

