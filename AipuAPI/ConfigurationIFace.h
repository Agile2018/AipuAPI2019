#ifndef ConfigurationIFace_h
#define ConfigurationIFace_h

#include "Configuration.h"
#include "ErrorFaceLib.h"


class ConfigurationIFace 
{
public:
	ConfigurationIFace();
	~ConfigurationIFace();
	void ParseJSONToObject();
	//void ParseMapToJSON();

	void SetMinFaceSize(int value) {
		minFaceSize = value;
	}
	void SetMaxFaceSize(int value) {
		maxFaceSize = value;
	}

	void SetMaxDetect(int maxFacesDetect) {
		maxfaceDetect = maxFacesDetect;
	}

	void SetConfidenceThreshold(int precision) {
		confidenceThreshold = precision;
	}	

	void SetQualityModel(int value) {
		qualityModel = value;
	}

	int GetQualityModel() {
		return qualityModel;
	}

	int GetMinFaceSize() {
		return minFaceSize;
	}

	int GetMaxFaceSize() {
		return maxFaceSize;
	}

	int GetMaxDetect() {
		return maxfaceDetect;
	}

	int GetConfidenceThreshold() {
		return confidenceThreshold;
	}
	
	int GetGetFaceCropImage() {
		return getFaceCropImage;
	}

	int GetSpeedAccurancyMode() {
		return speedAccurancyMode;
	}

	int GetFaceTemplextspeedAccurancyMode() {
		return faceTemplextspeedAccurancyMode;
	}	

	void SetStringJSON(string jsonString) {
		configuration->SetStringJSON(jsonString);
	}

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
private:
	
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	ErrorFaceLib* error = new ErrorFaceLib();
	Configuration* configuration = new Configuration();
	int minFaceSize = 25;
	int maxFaceSize = 200;
	int maxfaceDetect = 1;
	int confidenceThreshold = 600;	
	int qualityModel = 60;
	int getFaceCropImage = 1;
	int speedAccurancyMode = 0;
	int faceTemplextspeedAccurancyMode = 0;

	const string PARAMS = "paramsFaceProcessing";
	const string MAXFACESDETECT = "FACE_MAX_DETECT";
	const string MINFACE = "TRACK_MIN_FACE_SIZE";
	const string MAXFACE = "TRACK_MAX_FACE_SIZE";
	const string CONFIDENCE_THRESHOLD = "FACEDET_CONFIDENCE_THRESHOLD";
	const string SPEED_ACCURACY_MODE = "FACEDET_SPEED_ACCURACY_MODE";
	const string FACETMPLEXT_SPEED_ACCURACY_MODE = "FACETMPLEXT_SPEED_ACCURACY_MODE";
	const string FACE_CROP_IMAGE = "IFACE_GetFaceCropImage";
	const string CONFIGURATION = "configurationFaceProcessing";
	const string FACE_CONFIGURATION = "Face processing";
	const string QUALITYMODEL = "QUALITY_MODEL";


	void SetValueJSONToConfiguration();
	void ObserverError();
};


#endif // !ConfigurationIFace_h

