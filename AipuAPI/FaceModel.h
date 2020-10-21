#ifndef FaceModel_h
#define FaceModel_h

//#define BATCH_SIZE	5
#define BATCH_TOTAL_SIZE 30
#define EMPTY_FACE	0
#define TOTAL_FACE_DETECTED 50
//#define TRACKED_OBJECTS	5
//#define SIZE_COORDINATES 20
//#define NUMBER_COORDINATES_IMAGES  4

#include "ConfigurationIFace.h"
#include "ErrorFaceLib.h"
#include "Molded.h"  
#include <iostream>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;
namespace fs = boost::filesystem;


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
	
	void RecognitionFaceFiles(string namefile, int client, int task);
	void AddCollectionOfImages(string folder, int client, int doing);
	void InitHandle();
	void TerminateHandle();	

	void SetStringJSON(string jsonString) {
		configuration->SetStringJSON(jsonString);
	}

	void ParseJSONToObject() {
		configuration->ParseJSONToObject();
	}

	ConfigurationIFace* configuration = new ConfigurationIFace();
	Rx::subject<std::tuple<char*, vector<unsigned char>, int*, string>> templateImage;
	Rx::observable<std::tuple<char*, vector<unsigned char>, int*, string>> observableTemplate = templateImage.get_observable();
	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
	

private:
	
	ErrorFaceLib* error = new ErrorFaceLib();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	
	Rx::subscriber<std::tuple<char*, vector<unsigned char>, int*, string>> templateOut = templateImage.get_subscriber();
	std::vector<Molded*> modelsDetected;
	std::vector<std::vector<unsigned char>> bufferOfImagesBatch;
	std::vector<vector<int>> dimensionsImages;	

	bool isFinishLoadFiles = true;	
	void* faceHandlerGlobal = nullptr;	
	int countLowScore = 0;
	int countNotDetect = 0;
	int countErrorDetect = 0;
	int countDetect = 0;
	string tracerImage;
	std::vector<string> tracerProcess;
	File* file = new File();
	int ModelByBatch(int client, int doing);
	void FaceCropImage(void* face, Molded* model);
	int DetectByBatch(void* facesDetected[TOTAL_FACE_DETECTED],
		std::vector<std::vector<unsigned char>> bufferOfImagesBatch, int client, int doing);
	int DetectByBatch(void* facesDetected[TOTAL_FACE_DETECTED], int client, int doing);
	int GetBatchModels(int countFacesDetected, void* facesDetected[TOTAL_FACE_DETECTED]);
	void CreateTemplate(void* face, Molded* model, int client, int task);
	int GetOneModel(unsigned char* rawImage, int width, int height, int client, int task);
	//int ModelOneToOne(vector<unsigned char> buffer, int client);
	void ObserverError();
	vector<string> LoadFilesForBatch(string folder);
	void LoadImagesForBatch(vector<string> listFiles);
	unsigned char* LoadImage(vector<unsigned char> buffer, int *width, int *height);
	unsigned char* LoadFileImage(string image, int *width, int *height, int *length);
	string BuildTracerString();
	std::string IntToStr(int num);
};


#endif // !FaceModel_h

