#ifndef ConfigurationFile_h
#define ConfigurationFile_h

#include "Configuration.h"
#include "ErrorAipuLib.h"
//: public Configuration
class ConfigurationFile 
{
public:
	ConfigurationFile();
	~ConfigurationFile();
	void ParseJSONToObject();
	//void ParseMapToJSON();

	/*void SetNameFileConfigurationVideo(string name) {
		nameFileConfigurationVideo = name;
	}

	string GetNameFileConfigurationVideo() {
		return nameFileConfigurationVideo;
	}

	void SetNameFileConfigurationFaceModel(string name) {
		nameFileConfigurationFaceModel = name;
	}

	string GetNameFileConfigurationFaceModel() {
		return nameFileConfigurationFaceModel;
	}*/

	/*void SetNameDirectoryTempWork(string name) {
		nameDirectoryTempWork = name;
	}

	string GetNameDirectoryTempWork() {
		return nameDirectoryTempWork;
	}*/

	/*void SetNameDirectoryTraining(string name) {
		nameDirectoryTraining = name;
	}

	string GetNameDirectoryTraining() {
		return nameDirectoryTraining;
	}

	void SetNameFileConfigurationDatabase(string name) {
		nameFileConfigurationDatabase = name;
	}

	string GetNameFileConfigurationDatabase() {
		return nameFileConfigurationDatabase;
	}

	void SetNameFileConfigurationIdentify(string name) {
		nameFileConfigurationIdentify = name;
	}

	string GetNameFileConfigurationIdentify() {
		return nameFileConfigurationIdentify;
	}


	void SetNameFileConfigurationTracking(string name) {
		nameFileConfigurationTracking = name;
	}

	string GetNameFileConfigurationTracking() {
		return nameFileConfigurationTracking;
	}

	void SetNameFileConfigurationFlow(string name) {
		nameFileConfigurationFlow = name;
	}

	string GetNameFileConfigurationFlow() {
		return nameFileConfigurationFlow;
	}*/

	void SetNameFileConfiguration(string name) {
		configuration->SetNameFileConfiguration(name);
	}
	void SetNameDirectory(string name) {
		configuration->SetNameDirectory(name);
	}

	string GetStringJSON() {
		return configuration->GetStringJSON();
	}

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();

private:
	const string LABEL_ERROR_PARSING = "Error parsing the JSON string";
	ErrorAipuLib* error = new ErrorAipuLib();
	Configuration* configuration = new Configuration();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	void ObserverError();
	//void SetValueJSONToConfiguration();
	/*string nameFileConfigurationVideo;
	string nameFileConfigurationFaceModel;
	string nameFileConfigurationIdentify;
	string nameDirectoryTempWork;
	string nameDirectoryTraining;
	string nameFileConfiguration;
	string nameDirectoryConfiguration;
	string nameFileConfigurationDatabase;
	string nameFileConfigurationTracking;
	string nameFileConfigurationFlow;*/
	/*const string PARAMS = "Params";
	const string FILEVIDEO = "file_video";
	const string FILEFACE = "file_face";
	const string FILEDATABASE = "file_database";
	const string DIRECTORYWORK = "directory_work";
	const string DIRECTORYTRAIN = "directory_train";
	const string CONFIGURATION = "configuration";
	const string FILE_CONFIGURATION = "files_configuration";
	const string FILEIDENTIFY = "file_identify";
	const string FILETRACKING = "file_tracking";
	const string FILEFLOW = "file_flow";*/
};


#endif // !ConfigurationFile_h

