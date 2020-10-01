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
	//void ParseMapToJSON();
	//string ParseMapToJSONForIdentify();

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

	int GetFaceExtractionMode() {
		return faceExtractionMode;
	}

	void SetFaceExtractionMode(int value) {
		faceExtractionMode = value;
	}

	int GetIsRegister() {
		return isEnroll;
	}
	
	int GetDetectionThreshold() {
		return detectionThreshold;
	}	

	int GetSimilarityThresholdDeduplication() {
		return similarityThresholdDeduplication;
	}

	void SetSimilarityThresholdDeduplication(int value) {
		similarityThresholdDeduplication = value;
	}

	int GetIsDeduplication() {
		return isDeduplication;
	}

	void SetIsDeduplication(int value) {
		isDeduplication = value;
	}



	void SetStringJSON(string stringJson) {
		configuration->SetStringJSON(stringJson);
	}

	int GetMaximumConcatenateTemplates() {
		return maximumConcatenateTemplates;
	}

	int GetIsConcatenateTemplates() {
		return isConcatenateTemplates;
	}

	void SetIsConcatenateTemplates(int value) {
		isConcatenateTemplates = value;
	}

	int GetConcatenateMode() {
		return concatenateMode;
	}

	void SetConcatenateMode(int value) {
		concatenateMode = value;
	}

	int GetVerificationScore() {
		return verificationScore;
	}

	void SetVerificationScore(int value) {
		verificationScore = value;
	}

	int GetConcatenationMinimumScore() {
		return score_min;
	}

	void SetConcatenationMinimumScore(int value) {
		score_min = value;
	}

	int GetConcatenationMaximumScore() {
		return score_max;
	}

	void SetConcatenationMaximumScore(int value) {
		score_max = value;
	}

	int GetIdentificationThreshold() {
		return identificationThreshold;
	}

	void SetIdentificationThreshold(int value) {
		identificationThreshold = value;
	}

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
private:
	
	const string CONFIGURATION = "configurationEnrollmentProcessing";
	const string IDENTIFY_CONFIGURATION = "Enrollment processing";

	const string PARAMS = "paramsEnrollmentProcessing";

	const string CFG_BEST_CANDIDATES_COUNT = "CFG_BEST_CANDIDATES_COUNT";
	const string CFG_SIMILARITY_THRESHOLD = "CFG_SIMILARITY_THRESHOLD";	
	const string CFG_IDENTIFICATION_SPEED = "CFG_IDENTIFICATION_SPEED";
	const string CFG_IFACE_DETECT_FORCED = "CFG_IFACE_DETECT_FORCED";
	const string CFG_IFACE_IGNORE_MULTIPLE_FACES = "CFG_IFACE_IGNORE_MULTIPLE_FACES";
	const string CFG_IFACE_DETECTION_MODE = "CFG_IFACE_DETECTION_MODE";
	const string CFG_IFACE_EXTRACTION_MODE = "CFG_IFACE_EXTRACTION_MODE";
	const string CFG_IFACE_DETECTION_THRESHOLD = "CFG_IFACE_DETECTION_THRESHOLD";
	const string AFACE_PARAMETER_SCORE_MIN = "AFACE_PARAMETER_SCORE_MIN";
	const string AFACE_PARAMETER_SCORE_MAX = "AFACE_PARAMETER_SCORE_MAX";
	const string AFACE_PARAMETER_ENROLL = "AFACE_PARAMETER_ENROLL";
	/// <summary>
	/// //
	/// </summary>
	const string CFG_SIMILARITY_THRESHOLD_DEDUPLICATION = "CFG_SIMILARITY_THRESHOLD_DEDUPLICATION";
	const string AFACE_PARAMETER_DEDUPLICATION = "AFACE_PARAMETER_DEDUPLICATION";
	const string AFACE_PARAMETER_CONCATENATE_TEMPLATES = "AFACE_PARAMETER_CONCATENATE_TEMPLATES";
	const string AFACE_PARAMETER_MAXIMUM_TEMPLATES = "AFACE_PARAMETER_MAXIMUM_TEMPLATES";
	const string AFACE_PARAMETER_CONCATENATION_MODE = "AFACE_PARAMETER_CONCATENATION_MODE";
	const string AFACE_PARAMETER_VERIFICATION_SCORE = "AFACE_PARAMETER_VERIFICATION_SCORE";

	int identificationSpeed = 0;
	int faceDetectionForced = 1;
	int similarityThreshold = 40;
	int identificationThreshold = 40;
	int bestMatchedCandidates = 1;
	int detectionThreshold = 400;
	int score_min = 30;
	int score_max = 60;
	int isEnroll = 1;
	int ignoreMultipleFaces = 1;
	int faceDetectionMode = 2;
	int faceExtractionMode = 2;
	int similarityThresholdDeduplication = 0;
	int isDeduplication = 1;
	int maximumConcatenateTemplates = -1;
	int isConcatenateTemplates = 1;
	int concatenateMode = 0;
	int verificationScore = 20;
	
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	ErrorFaceLib* error = new ErrorFaceLib();
	Configuration* configuration = new Configuration();
	void SetValueJSONToConfiguration();
	void ObserverError();
};



#endif	