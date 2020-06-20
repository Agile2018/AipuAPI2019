#include "ConfigurationDatabase.h"

ConfigurationDatabase::ConfigurationDatabase()
{
	ObserverError();
}

ConfigurationDatabase::~ConfigurationDatabase()
{
}

void ConfigurationDatabase::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
		shootError.on_next(either);
	});
}

void ConfigurationDatabase::SetValueJSONToConfiguration() {
	const Json::Value params = configuration->jsonBody[PARAMS];
	for (Json::Value::const_iterator it = params.begin();
		it != params.end(); ++it) {

		if (it.key().asString() == NAME_DATABASE) {
			nameDatabase = it->asString();
		}
		else if (it.key().asString() == CONNECT_STRING) {
			if (!it->asString().empty()) {
				connectString = it->asString();
			}
		}

	}

}

void ConfigurationDatabase::ParseJSONToObject() {
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

void ConfigurationDatabase::ParseMapToJSON() {
	configuration->jsonBody.clear();
	configuration->jsonParams.clear();
	std::map<std::string, std::string> params;
	params.insert(std::pair<std::string, std::string>(NAME_DATABASE, nameDatabase));
	params.insert(std::pair<std::string, std::string>(CONNECT_STRING, connectString));

	std::map<std::string, std::string>::const_iterator it = params.begin(),
		end = params.end();
	for (; it != end; ++it) {
		configuration->jsonParams[it->first] = it->second;

	}

	configuration->jsonBody[CONFIGURATION] = DATABASE_CONFIGURATION;
	configuration->jsonBody[PARAMS] = configuration->jsonParams;

	configuration->SaveConfiguration();

}
