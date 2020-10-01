#include "ConfigurationFile.h"

ConfigurationFile::ConfigurationFile()
{
	ObserverError();
}

ConfigurationFile::~ConfigurationFile()
{
}

void ConfigurationFile::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([this](Either* either) {		
		shootError.on_next(either);
	});
}

//void ConfigurationFile::ParseMapToJSON() {
//	configuration->jsonBody.clear();
//	configuration->jsonParams.clear();
//	std::map<std::string, std::string> params;
//	params.insert(std::pair<std::string, std::string>(FILEVIDEO, nameFileConfigurationVideo));
//	params.insert(std::pair<std::string, std::string>(FILEFACE, nameFileConfigurationFaceModel));
//	params.insert(std::pair<std::string, std::string>(DIRECTORYWORK, nameDirectoryTempWork));
//	params.insert(std::pair<std::string, std::string>(DIRECTORYTRAIN, nameDirectoryTraining));
//	params.insert(std::pair<std::string, std::string>(FILEDATABASE, nameFileConfigurationDatabase));
//	params.insert(std::pair<std::string, std::string>(FILEIDENTIFY, nameFileConfigurationIdentify));	
//	params.insert(std::pair<std::string, std::string>(FILETRACKING, nameFileConfigurationTracking));
//	params.insert(std::pair<std::string, std::string>(FILEFLOW, nameFileConfigurationFlow));
//
//	std::map<std::string, std::string>::const_iterator it = params.begin(),
//		end = params.end();
//	for (; it != end; ++it) {
//		configuration->jsonParams[it->first] = it->second;
//
//	}
//
//	configuration->jsonBody[CONFIGURATION] = FILE_CONFIGURATION;
//	configuration->jsonBody[PARAMS] = configuration->jsonParams;
//
//	configuration->SaveConfiguration();
//
//}

//void ConfigurationFile::SetValueJSONToConfiguration() {
//	const Json::Value params = configuration->jsonBody[PARAMS];
//	for (Json::Value::const_iterator it = params.begin();
//		it != params.end(); ++it) {
//
//		if (it.key().asString() == FILEVIDEO) {
//			nameFileConfigurationVideo = it->asString();
//		}
//		else if (it.key().asString() == FILEFACE) {
//			if (!it->asString().empty()) {
//				nameFileConfigurationFaceModel = it->asString();
//			}
//		}
//		else if (it.key().asString() == FILEIDENTIFY) {
//			if (!it->asString().empty()) {
//				nameFileConfigurationIdentify = it->asString();
//			}
//		}
//
//		else if (it.key().asString() == FILETRACKING) {
//			if (!it->asString().empty()) {
//				nameFileConfigurationTracking = it->asString();
//			}
//		}
//		else if (it.key().asString() == FILEFLOW) {
//			if (!it->asString().empty()) {
//				nameFileConfigurationFlow = it->asString();
//			}
//		}
//
//		else if (it.key().asString() == DIRECTORYWORK) {
//			if (!it->asString().empty()) {
//				nameDirectoryTempWork = it->asString();
//			}
//		}		
//		else if (it.key().asString() == FILEDATABASE) {
//			if (!it->asString().empty()) {
//				nameFileConfigurationDatabase = it->asString();
//			}
//		}
//		else if (it.key().asString() == DIRECTORYTRAIN) {
//			nameDirectoryTraining = it->asString();
//		}
//	}
//
//}

void ConfigurationFile::ParseJSONToObject() {
	configuration->jsonBody.clear();
	configuration->stringJSON = configuration->manageFile->ReadFileText();
	bool parsingSuccessful = configuration->reader.parse(configuration->stringJSON, configuration->jsonBody);
	if (!parsingSuccessful)
	{
		error->CheckError(ERROR_PARSING, error->medium, LABEL_ERROR_PARSING);
	}
	
}

