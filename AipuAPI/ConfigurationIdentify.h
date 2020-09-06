#ifndef ConfigurationIdentify_h
#define ConfigurationIdentify_h

#include "Configuration.h"
#include "ErrorFaceLib.h"

class ConfigurationIdentify
{
public:
	ConfigurationIdentify();
	~ConfigurationIdentify();
	void ParseJSONToObject();
	void ParseMapToJSON();
	string ParseMapToJSONForIdentify();

	void SetMinEyeDistance(int minDistance) {
		minEyeDistance = minDistance;
	}
	void SetMaxEyeDistance(int maxDistance) {
		maxEyeDistance = maxDistance;
	}

	void SetIdentificationSpeed(int speed) {
		identificationSpeed = speed;
	}

	void SetSimilarityThreshold(int threshold) {
		similarityThreshold = threshold;
	}

	void SetBestMatchedCandidates(int best) {
		bestMatchedCandidates = best;
	}

	void SetFaceDetectionForced(int forced) {
		faceDetectionForced = forced;
	}

	int GetBestMatchedCandidates() {
		return bestMatchedCandidates;
	}

	int GetSimilarityThreshold() {
		return similarityThreshold;
	}
	

	int GetIdentificationSpeed() {
		return identificationSpeed;
	}

	int GetFaceDetectionForced() {
		return faceDetectionForced;
	}

	int GetMinEyeDistance() {
		return minEyeDistance;
	}

	int GetMaxEyeDistance() {
		return maxEyeDistance;
	}

	int GetBiometricLogLevel() {
		return biometricLogLevel;
	}

	void SetBiometricLogLevel(int value) {
		biometricLogLevel = value;
	}

	int GetIgnoreMultipleFaces() {
		return ignoreMultipleFaces;
	}

	void SetIgnoreMultipleFaces(int value) {
		ignoreMultipleFaces = value;
	}

	int GetFaceDetectionMode() {
		return faceDetectionMode;
	}

	void SetFaceDetectionMode(int value) {
		faceDetectionMode = value;
	}

	int GetSearchExtractionThreads() {
		return searchExtractionThreads;
	}

	void SetSearchExtractionThreads(int value) {
		searchExtractionThreads = value;
	}

	int GetFaceExtractionMode() {
		return faceExtractionMode;
	}

	void SetFaceExtractionMode(int value) {
		faceExtractionMode = value;
	}

	int GetIsRegister() {
		return isRegister;
	}

	void SetNameFileConfiguration(string name) {
		configuration->SetNameFileConfiguration(name);
	}
	void SetNameDirectory(string name) {
		configuration->SetNameDirectory(name);
	}

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
private:
	const int MIN_EYE_DISTANCE = 25;
	const int MAX_EYE_DISTANCE = 250;
	const int IDENTIFICATION_SPEED = 0; //*
	const int FACE_DETECTION_FORCED = 0;
	const int SIMILARITY_THRESHOLD = 40; //0..1000 *
	const int BEST_MATCHED_CANDIDATES = 1; //*
	const int BIOMETRIC_LOG_LEVEL = 0;
	const int IGNORE_MULTIPLE_FACES = 0;
	const int FACE_DETECTION_MODE = 1;
	const int SEARCH_EXTRACTION_THREADS = 0;
	const int FACE_EXTRACTION_MODE = 1;
	const int IS_REGISTER = 1;

	const string CONFIGURATION = "configuration";
	const string IDENTIFY_CONFIGURATION = "identify_configuration";

	const string PARAMS = "Params";
	const string IDENTIFICATIONSPEED = "A_IdentificationSpeed";
	const string FACEDETECTIONFORCED = "A_FaceDetectionForced";	
	const string AMINEYE = "A_MinEyeDist";
	const string AMAXEYE = "A_MaxEyeDist";
	const string SIMILARITYTHRESHOLD = "A_SimilarityThreshold";
	const string BESTMATCHEDCANDIDATES = "A_BestMatchedCandidates";
	const string BIOMETRICLOGLEVEL = "A_BiometricLogLevel";
	const string IGNOREMULTIPLEFACES = "A_IgnoreMultipleFaces";
	const string FACEDETECTIONMODE = "A_FaceDetectionMode";
	const string SEARCHEXTRACTIONTHREADS = "A_SearchorExtractionThreads";
	const string FACEEXTRACTIONMODE = "A_FaceExtractionMode";
	const string ISREGISTER = "is_register";

	int minEyeDistance = MIN_EYE_DISTANCE;
	int maxEyeDistance = MAX_EYE_DISTANCE;
	int identificationSpeed = IDENTIFICATION_SPEED;
	int faceDetectionForced = FACE_DETECTION_FORCED;
	int similarityThreshold = SIMILARITY_THRESHOLD;
	int bestMatchedCandidates = BEST_MATCHED_CANDIDATES;
	int biometricLogLevel = BIOMETRIC_LOG_LEVEL;
	int ignoreMultipleFaces = IGNORE_MULTIPLE_FACES;
	int faceDetectionMode = FACE_DETECTION_MODE;
	int searchExtractionThreads = SEARCH_EXTRACTION_THREADS;
	int faceExtractionMode = FACE_EXTRACTION_MODE;
	int isRegister = IS_REGISTER;
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	ErrorFaceLib* error = new ErrorFaceLib();
	Configuration* configuration = new Configuration();
	void SetValueJSONToConfiguration();
	void ObserverError();
};



#endif	