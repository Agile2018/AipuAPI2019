#include "Innovatrics.h"

Innovatrics::Innovatrics()
{
	ObserverError();	
	VerifyProcessorGraphic();
}

Innovatrics::~Innovatrics()
{
	IFACE_Terminate();

}

void Innovatrics::VerifyProcessorGraphic()
{
	isGraphicProcessor = graphicProcessor->ThereIsGraphicProcessor();
	
}

void Innovatrics::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
		shootError.on_next(either);
	});
}

void Innovatrics::InitLibrary() {
	int errorCode;		
	
	errorCode = IFACE_Init();
	if (errorCode != IFACE_OK) {
		error->CheckError(errorCode, error->gross);
	}
	cout << "initLibrary IFACE_Init " << errorCode << endl;
	
	auto initData = "{\"licenseFile\":\"iengine.lic\"}";
	errorCode = initLibrary(initData);
	if (errorCode != IFACE_OK) {
		error->CheckError(errorCode, error->gross);
	}
	cout << "initLibrary " << errorCode << endl;
	std::string str = "iengine.db";
	errorCode = connectToDatabase(str.c_str());
	cout << "connectToDatabase returns " << errorCode << endl;	
}

//void Innovatrics::InitLibraryIdentify() {
//	/*int errorCode;
//	auto initData = "{\"licenseFile\":\"iengine.lic\"}";
//	errorCode = initLibrary(initData);
//	if (errorCode != IFACE_OK) {
//		error->CheckError(errorCode, error->gross);
//	}
//	cout << "initLibrary initLibrary " << errorCode << endl;*/
//}

bool Innovatrics::InitParamsGraphicProcessor() {
	int errorCode;
	errorCode = IFACE_SetParam(nullptr,
		IFACE_PARAMETER_GLOBAL_GPU_ENABLED, "true");
	if (errorCode == IFACE_OK) {
		errorCode = IFACE_SetParam(nullptr,
			IFACE_PARAMETER_GLOBAL_GPU_DEVICE_ID, "0");
		if (errorCode != IFACE_OK) {
			error->CheckError(errorCode, error->less);
			return false;
		}
	}
	else {
		error->CheckError(errorCode, error->less);
		return false;
	}
	return true;
}

void Innovatrics::SetParamsLibrary() {
	int errorCode;
	if (isGraphicProcessor) {
		if (InitParamsGraphicProcessor())
		{
			cout << "CUDA SET INNOVATRICS OK." << endl;
		}
		else {
			cout << "CUDA SET INNOVATRICS ERROR." << endl;
		}
	}
	else {
		cout << "CUDA NO EXISTE O NO HA SIDO SELECCIONADO." << endl;
	}
	errorCode = IFACE_SetParam(IFACE_GLOBAL_PARAMETERS,
		IFACE_PARAMETER_GLOBAL_THREAD_MANAGEMENT_MODE,
		IFACE_GLOBAL_THREAD_MANAGEMENT_MODE_MIN_MEMORY);
	if (errorCode != IFACE_OK) {
		error->CheckError(errorCode, error->medium);
	}
	errorCode = IFACE_SetParam(IFACE_GLOBAL_PARAMETERS,
		IFACE_PARAMETER_GLOBAL_THREAD_NUM, "4"); //IFACE_GLOBAL_THREAD_NUM_DEFAULT  "2"
	if (errorCode != IFACE_OK) {
		error->CheckError(errorCode, error->medium);
	}	

}

void Innovatrics::Terminate() {
	int errorCode;

	errorCode = IFACE_Terminate();
	if (errorCode != IFACE_OK) {
		error->CheckError(errorCode, error->gross);
	}
	
	errorCode = endLibrary();
	if (errorCode != IFACE_OK) {
		error->CheckError(errorCode, error->gross);
	}
}