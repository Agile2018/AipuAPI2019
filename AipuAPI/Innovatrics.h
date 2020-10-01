#ifndef Innovatrics_h
#define Innovatrics_h

#include "ErrorFaceLib.h"
#include "GraphicProcessor.h"
#include "FaceIdkit.h"
#include "ConfigurationGlobal.h"

class Innovatrics
{
public:
	Innovatrics();
	~Innovatrics();
	void InitLibrary();
	
	void SetIsGraphicProcessor(bool processor) {
		isGraphicProcessor = processor;
	}
	void SetParamsLibrary();
	void Terminate();

	void SetNameFileConfiguration(string name) {
		configurationGlobal->SetNameFileConfiguration(name);
	}
	void SetNameDirectory(string name) {
		configurationGlobal->SetNameDirectory(name);
	}

	void ParseJSONToObject() {
		configurationGlobal->ParseJSONToObject();
	}

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();

private:
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	ErrorFaceLib* error = new ErrorFaceLib();
	FaceIdkit* faceIdkit = new FaceIdkit();
	ConfigurationGlobal* configurationGlobal = new ConfigurationGlobal();
	bool InitParamsGraphicProcessor();
	bool isGraphicProcessor = false;
	void ObserverError();
	GraphicProcessor* graphicProcessor = new GraphicProcessor();
	void VerifyProcessorGraphic();
	string connectDatabase = "iengine.db";
};


#endif // !Innovatrics_h

