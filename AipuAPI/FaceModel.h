#ifndef FaceModel_h
#define FaceModel_h

#define BATCH_SIZE	5
#define BATCH_TOTAL_SIZE 30
#define EMPTY_FACE	0
#define TRACKED_OBJECTS	5
#define SIZE_COORDINATES 20
#define NUMBER_COORDINATES_IMAGES  4

#include "ConfigurationIFace.h"
#include "ErrorFaceLib.h"
#include "Molded.h"

class FaceModel
{
public:
	FaceModel();
	~FaceModel();
	
	void ResetLowScore() {
		countLowScore = 0;
	}

	int GetCountLowScore() {
		return countLowScore;
	}

	void ResetCountNotDetect() {
		countNotDetect = 0;
	}

	int GetCountNotDetect() {
		return countNotDetect;
	}

	int GetCountErrorDetect() {
		return countErrorDetect;
	}

	void ResetCountErrorDetect() {
		countErrorDetect = 0;
	}

	int GetCountDetect() {
		return countDetect;
	}

	void ResetCountDetect() {
		countDetect = 0;
	}

	void SetIsFinishLoadFiles(bool value) {
		isFinishLoadFiles = value;
	}

	bool GetIsFinishLoadFiles() {
		return isFinishLoadFiles;
	}
	
	void Terminate();
	
	int ModelByBatch(std::vector<std::vector<unsigned char>> bufferOfImagesBatch,
		std::vector<int> clients);
	int ModelOneToOne(vector<unsigned char> buffer, int client);
	void RecognitionFaceFiles(string file, int client);
	
	void InitHandle();
	void TerminateHandle();
	void ProcessFaceTracking(std::tuple<char*, unsigned char*, int*> dataSerialize, int client);
	ConfigurationIFace* configuration = new ConfigurationIFace();
	Rx::subject<std::tuple<char*, vector<unsigned char>, int*>> templateImage;
	Rx::observable<std::tuple<char*, vector<unsigned char>, int*>> observableTemplate = templateImage.get_observable();
	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
	

private:
	
	ErrorFaceLib* error = new ErrorFaceLib();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	
	Rx::subscriber<std::tuple<char*, vector<unsigned char>, int*>> templateOut = templateImage.get_subscriber();
	std::vector<Molded*> modelsDetected;
	
	bool isFinishLoadFiles = true;	
	void* faceHandlerGlobal = nullptr;	
	int countLowScore = 0;
	int countNotDetect = 0;
	int countErrorDetect = 0;
	int countDetect = 0;

	void FaceCropImage(void* face, Molded* model);
	int DetectByBatch(void* facesDetected[BATCH_TOTAL_SIZE],
		std::vector<std::vector<unsigned char>> bufferOfImagesBatch, std::vector<int> clients);
	void GetBatchModels(int countFacesDetected, void* facesDetected[BATCH_TOTAL_SIZE]);
	void CreateTemplate(void* face, Molded* model, int client);
	int GetOneModel(unsigned char* rawImage, int width, int height, int client);
	
	void ObserverError();
	
	unsigned char* LoadImage(vector<unsigned char> buffer, int *width, int *height);
	unsigned char* LoadFileImage(string image, int *width, int *height, int *length);
	
	std::string IntToStr(int num);
};


#endif // !FaceModel_h

