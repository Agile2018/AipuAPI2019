#include "Database.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

Database::Database()
{
	ObserverError();
	MongoAccess::instance().ObserverError();
}

Database::~Database()
{
	delete file;
	delete base64;
	delete error;
}

void Database::Configure() {	
	mongocxx::uri uri(configuration->GetConnectString().c_str()); //"mongodb://localhost:27017"
	class NoopLogger : public mongocxx::logger {
	public:
		virtual void operator()(mongocxx::log_level,
			bsoncxx::stdx::string_view,
			bsoncxx::stdx::string_view) noexcept {}
	};

	auto instance =
		bsoncxx::stdx::make_unique<mongocxx::instance>(bsoncxx::stdx::make_unique<NoopLogger>());

	MongoAccess::instance().Configure(std::move(instance),
		bsoncxx::stdx::make_unique<mongocxx::pool>(std::move(uri)));

}

void Database::CheckDatabase() {
	auto clientConnection = MongoAccess::instance().GetConnection();

	mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];

	auto cursor = database.list_collections();
		
	for (const bsoncxx::document::view& doc : cursor)
	{
		databaseIsCreated = true;
		bsoncxx::document::element ele = doc["name"];
		
	}

	/*if (databaseIsCreated)
	{
		DeleteRecordsUserUnidentified();
	}*/
}

void Database::InsertNewUser(User* user) {
	
	AddUser(user);
	AddImageUser(user->GetCropImageData(), user->GetMoldCropHeight(), user->GetMoldCropWidth(),
		user->GetUserIdIFace(), user->GetLogProcess());

	/*std::thread(&Database::AddUser, this, user).join();
	std::thread(&Database::AddImageUser, this,
		user->GetCropImageData(), user->GetMoldCropHeight(), user->GetMoldCropWidth(),
		user->GetUserIdIFace()).join();*/
	
	BuildNewUser(user);
	
}

//void Database::AddRecordsUser(User* user) {
//	AddImageUser(user->GetCropImageData(), user->GetMoldCropHeight(), 
//		user->GetMoldCropWidth(), user->GetUserIdIFace());		
//	AddUser(user);
//	BuildNewUser(user);
//}

void Database::DeleteRecordsImagesUnidentified() {

	if (databaseIsCreated)
	{
		try
		{
			auto clientConnection = MongoAccess::instance().GetConnection();

			mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];

			mongocxx::collection collection = database[COLLECTION_IMAGE.c_str()];

			bsoncxx::document::value builder = make_document(
				kvp("id_face", make_document(kvp("$lt", 0))));

			boost::optional<mongocxx::v_noabi::result::delete_result> result = collection.delete_many(std::move(builder));


			if (result) {
				std::cout << result->deleted_count() << "\n";
			}
		}
		catch (const mongocxx::exception& e)
		{
			error->CheckError(ERROR_DATABASE,
				error->medium, e.what());

		}
	}
	
}

void Database::DeleteRecordsUserUnidentified() {

	if (databaseIsCreated) {
		try
		{
			auto clientConnection = MongoAccess::instance().GetConnection();

			mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];

			mongocxx::collection collection = database[COLLECTION_USER.c_str()];

			bsoncxx::document::value builder = make_document(
				kvp("id_face", make_document(kvp("$lt", 0))));

			boost::optional<mongocxx::v_noabi::result::delete_result> result = collection.delete_many(std::move(builder));


			if (result) {
				std::cout << result->deleted_count() << "\n";
			}
		}
		catch (const mongocxx::exception& e)
		{
			error->CheckError(ERROR_DATABASE,
				error->medium, e.what());

		}
	}
	
}

void Database::AddUser(User* user) {
		
	try
	{
		auto clientConnection = MongoAccess::instance().GetConnection();		

		mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];

		mongocxx::collection collection = database[COLLECTION_USER.c_str()];

		/*lastUserId = user->GetUserIdIFace();
		lastClient = user->GetClient();*/
		bsoncxx::document::value builder = make_document(
			kvp("id_face", user->GetUserIdIFace()),
			kvp("name", user->GetNameUser().c_str()),
			kvp("lastname", user->GetLastNameUser().c_str()),
			kvp("identification", user->GetIdentificationUser().c_str()));
	
		bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(std::move(builder));
		databaseIsCreated = true;
	}
	catch (const mongocxx::exception& e)
	{
		error->CheckError(ERROR_DATABASE,
			error->medium, e.what());
		
	}

}


void Database::AddImageUser(vector<unsigned char> image, int rows, int cols, int idUser, string log) {

	try
	{
		auto clientConnection = MongoAccess::instance().GetConnection();		

		mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];
		mongocxx::collection collection = database[COLLECTION_IMAGE.c_str()];
		
		string imageBase64 = FileImageToStringBase64(image, rows, cols);		

		bsoncxx::document::value builder = make_document(
			kvp("id_face", idUser),
			kvp("data_64", imageBase64.c_str()),
			kvp("data_64_aux", "empty"), 
			kvp("log", log.c_str()));
	
		bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(std::move(builder));

	}
	catch (const mongocxx::exception& e)
	{
		error->CheckError(ERROR_DATABASE,
			error->medium, e.what());
	}
	
}

void Database::BuildNewUser(User* user) {
	int idFace = user->GetUserIdIFace();
	int client = user->GetClient();
	int score = user->GetMoldScore();
	std::vector<std::string> values;
	values.push_back(to_string(idFace));
	values.push_back(user->GetNameUser());
	values.push_back(user->GetLastNameUser());
	values.push_back(user->GetIdentificationUser());
	values.push_back(to_string(user->GetStateUser()));	
	values.push_back(to_string(client));
	values.push_back(to_string(score));
	BuildJSONUser(values);
}

string Database::FileImageToStringBase64(vector<unsigned char> image, int rows, int cols) {
	string encodedPng;
	vector<uchar> bufferImage;
	//Mat img = imread(path, IMREAD_COLOR);
	cv::Mat img = cv::Mat(rows, cols, CV_8UC3);
	img.data = &image[0];

	int params[3] = { 0 };
	params[0] = cv::IMWRITE_JPEG_QUALITY;
	params[1] = 100;

	if (!img.empty())
	{
		try
		{
			bool code = cv::imencode(".jpg", img,
				bufferImage, std::vector<int>(params, params + 2));
			uchar* buffToBase64 = reinterpret_cast<uchar*> (&bufferImage[0]);

			encodedPng = base64->base64_encode(buffToBase64,
				(unsigned int)bufferImage.size());
		}
		catch (cv::Exception& e)
		{
			//const char* err_msg = e.what();
			std::cout << "Exception caught: " << e.what() << std::endl;
		}
		catch (const std::exception& ex) {
			std::cout << "Exception runtime: " << ex.what() << std::endl;
		}
		//std::thread(&Database::DeleteFileTempCropImage, this, path).detach();
	}
	return encodedPng;
}

void Database::DeleteFileTempCropImage(string filePath) {
	file->DeleteFile(filePath);
}

void Database::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
		shootError.on_next(either);
	});

	auto observerErrorDatabase = MongoAccess::instance().observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionErrorDatabase = observerErrorDatabase.subscribe([this](Either* either) {
		shootError.on_next(either);
	});
	auto observerConfiguration = configuration->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionConfiguration = observerConfiguration.subscribe([this](Either* either) {
		shootError.on_next(either);
	});



}

void Database::FindUserByIdFace(User* user) {	

	if (QueryUserByFace(user->GetUserIdIFace(), user->GetClient(), user->GetMoldScore()))
	{
		
		UpdateImageUser(user->GetUserIdIFace(), user->GetCropImageData(), 
			user->GetMoldCropHeight(), user->GetMoldCropWidth(), user->GetLogProcess());
	}
	/*std::thread(&Database::QueryUserByFace, this, idFaceUser, client).detach();
	std::thread(&Database::UpdateImageUser, this, idFaceUser, image, rows, cols).detach();*/
}

bool Database::QueryUserByFace(int idFaceUser, int client, int score) {

	/*if (lastUserId != idFaceUser || lastClient != client)
	{*/
		//countRepeatOfSomeUser = 0;
	
	bool flagSearch = false;

	if (databaseIsCreated)
	{
		try
		{
			
			auto clientConnection = MongoAccess::instance().GetConnection();
			
			mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];
			mongocxx::collection collection = database[COLLECTION_USER.c_str()];
			/*lastUserId = idFaceUser;
			lastClient = client;*/
			
			boost::optional<bsoncxx::v_noabi::document::value> cursor = collection
				.find_one(make_document(kvp("id_face", idFaceUser)));
			
			if (cursor)
			{				
				auto view = (*cursor).view();
								
				if (!view.empty()) {
					
					flagSearch = true;
					int idFace = view["id_face"].get_int32().value;
					std::vector<std::string> values;
					values.push_back(to_string(idFace));
					values.push_back(view["name"].get_utf8().value.to_string());
					values.push_back(view["lastname"].get_utf8().value.to_string());
					values.push_back(view["identification"].get_utf8().value.to_string());
					values.push_back("2");
					values.push_back(to_string(client));
					values.push_back(to_string(score));
					BuildJSONUser(values);
				}
			}
			
		}
		catch (const mongocxx::exception& e)
		{
			error->CheckError(ERROR_DATABASE,
				error->medium, e.what());			
		}
	}
	else
	{
		string messageError = "Mongo database does not exist.";
		error->CheckError(ERROR_DATABASE,
			error->medium, messageError);
	}
			
	return flagSearch;
	/*}
	else {
		countRepeatOfSomeUser += 1;
		if (countRepeatOfSomeUser > 2)
		{			
			lastUserId = -1;
		}
	}*/

}

void Database::UpdateImageUser(int idFaceUser, vector<unsigned char> image, int rows, int cols, string log) {
	
	auto clientConnection = MongoAccess::instance().GetConnection();

	mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];
	mongocxx::collection collection = database[COLLECTION_IMAGE.c_str()];
	
	string imageBase64 = FileImageToStringBase64(image, rows, cols);	
	
	if (!imageBase64.empty())
	{
		try
		{

			bsoncxx::stdx::optional<mongocxx::v_noabi::result::update> result = collection
				.update_one(make_document(kvp("id_face", idFaceUser)),
					make_document(kvp("$set", make_document(kvp("data_64_aux", imageBase64), kvp("log", log)))));
		}
		catch (const mongocxx::exception& e)
		{
			error->CheckError(ERROR_DATABASE,
				error->medium, e.what());
		}
	}
	
}


void Database::BuildJSONUser(vector<std::string> values) {
	string stringJSON;
	Json::Value jsonParams;
	Json::Value jsonBody;

	std::map<std::string, std::string> params;
	params.insert(std::pair<std::string, std::string>(FIELD_USER_ID_FACE, values[0]));
	params.insert(std::pair<std::string, std::string>(FIELD_USER_NAME, values[1]));
	params.insert(std::pair<std::string, std::string>(FIELD_USER_LAST_NAME, values[2]));
	params.insert(std::pair<std::string, std::string>(FIELD_USER_IDENTIFICATION, values[3]));
	params.insert(std::pair<std::string, std::string>(FIELD_USER_REGISTER, values[4]));
	params.insert(std::pair<std::string, std::string>(FIELD_CLIENT, values[5]));
	params.insert(std::pair<std::string, std::string>(FIELD_SCORE, values[6]));
	std::map<std::string, std::string>::const_iterator it = params.begin(),
		end = params.end();
	for (; it != end; ++it) {
		jsonParams[it->first] = it->second;

	}

	jsonBody[COLLECTION_USER] = DATASOURCE;
	jsonBody[PARAMS] = jsonParams;

	Json::StreamWriterBuilder builder;
	builder.settings_["indentation"] = "    ";
	stringJSON = Json::writeString(builder, jsonBody);
	shootUserJSON.on_next(stringJSON);

}

string Database::QueryImageOfUser(int idFaceUser) {
	auto clientConnection = MongoAccess::instance().GetConnection();
	mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];
	mongocxx::collection collection = database[COLLECTION_IMAGE.c_str()];
	boost::optional<bsoncxx::v_noabi::document::value> cursor = collection
		.find_one(make_document(kvp("id_face", idFaceUser)));
	auto view = (*cursor).view();
	string image = view["data_64"].get_utf8().value.to_string();
	return image;
}

void Database::ProcessUserDB(User* user) {
	if (user->GetStateUser() == 1)
	{
		string number = to_string(user->GetUserIdIFace());
		string name = "Person " + number;
		string lastName = "LasName " + number;
		string identification = "0000000";
		user->SetNameUser(name);
		user->SetLastNameUser(lastName);
		user->SetIdentificationUser(identification);
		InsertNewUser(user);

	}
	else if (user->GetStateUser() == 2) {
		
		FindUserByIdFace(user);
		
	}
	else if (user->GetStateUser() == 3) {
		string name = "Unidentified";
		string lastName = "Unidentified";
		string identification = "-";
		user->SetNameUser(name);
		user->SetLastNameUser(lastName);
		user->SetIdentificationUser(identification);
		InsertNewUser(user);
	}

	
}