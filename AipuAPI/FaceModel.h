#ifndef FaceModel_h
#define FaceModel_h

//#define BATCH_SIZE	5
#define BATCH_TOTAL_SIZE 30
#define EMPTY_FACE	0
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
	
	void RecognitionFaceFiles(string file, int client);
	void AddCollectionOfImages(string folder, int client, int doing);
	void InitHandle();
	void TerminateHandle();	
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
	std::vector<std::vector<unsigned char>> bufferOfImagesBatch;
	std::vector<vector<int>> dimensionsImages;
	bool isFinishLoadFiles = true;	
	void* faceHandlerGlobal = nullptr;	
	int countLowScore = 0;
	int countNotDetect = 0;
	int countErrorDetect = 0;
	int countDetect = 0;
	int ModelByBatch(int client, int doing);
	void FaceCropImage(void* face, Molded* model);
	int DetectByBatch(void* facesDetected[BATCH_TOTAL_SIZE],
		std::vector<std::vector<unsigned char>> bufferOfImagesBatch, int client, int doing);
	int GetBatchModels(int countFacesDetected, void* facesDetected[BATCH_TOTAL_SIZE]);
	void CreateTemplate(void* face, Molded* model, int client);
	int GetOneModel(unsigned char* rawImage, int width, int height, int client);
	int ModelOneToOne(vector<unsigned char> buffer, int client);
	void ObserverError();
	vector<string> LoadFilesForBatch(string folder);
	void LoadImagesForBatch(vector<string> listFiles);
	unsigned char* LoadImage(vector<unsigned char> buffer, int *width, int *height);
	unsigned char* LoadFileImage(string image, int *width, int *height, int *length);
	
	std::string IntToStr(int num);
};


#endif // !FaceModel_h

