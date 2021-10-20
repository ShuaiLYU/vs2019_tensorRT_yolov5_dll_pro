
#include "app_yolo.hpp"

#include <iostream>  

//#include "yaml-cpp/yaml.h"
using namespace std;
string MODEL_NAME;
string INFER_TYPE;
int BATCH_SIZE ;
float OBJ_THRESHOLD;
float NMS_THRESHOLD ;
string WORK_SPACE;
int  DEVICE_ID;
bool MODE_DEBUG=true;

static const char* cocolabels[] = {
	"0", "1"
};


static void append_to_file(const string& file, const string& data) {
	FILE* f = fopen(file.c_str(), "a+");
	if (f == nullptr) {
		INFOE("Open %s failed.", file.c_str());
		return;
	}

	fprintf(f, "%s\n", data.c_str());
	fclose(f);
}

std::vector<Defects> inference_and_performance(shared_ptr<Yolo::Infer>engine, vector<cv::Mat> images) {



	std::vector<Defects> batch_results;
	
	vector<shared_future<Yolo::BoxArray>> boxes_array;

	auto begin_timer = iLogger::timestamp_now_float();
	boxes_array = engine->commits(images);

	// wait all result
	boxes_array.back().get();

	float inference_average_time = (iLogger::timestamp_now_float() - begin_timer) / images.size();
	const char* mode_name;
	if (INFER_TYPE == "FP32")
	{
		mode_name = TRT::mode_string(TRT::Mode::FP32);
	}
	else if (INFER_TYPE == "FP16")
	{
		mode_name = TRT::mode_string(TRT::Mode::FP16);
	}


	INFO("%s average: %.2f ms / image, FPS: %.2f", mode_name, inference_average_time, 1000 / inference_average_time);
	append_to_file(WORK_SPACE+"perf.result.log", iLogger::format("%s,%s,%s,%f", MODEL_NAME.c_str(), INFER_TYPE.c_str(), mode_name, inference_average_time));

	string root = WORK_SPACE+iLogger::format("%s_%s_result", MODEL_NAME.c_str(), INFER_TYPE.c_str());
	iLogger::rmtree(root);
	iLogger::mkdir(root);

	for (int i = 0; i < boxes_array.size(); ++i) {

		auto& image = images[i];
		auto boxes = boxes_array[i].get();


		Defects image_results;


		////test 
		//Defect l_def;
		//l_def.rect = cv::Rect(0, 0, 10, 20);
		//l_def.cls = 1;
		//image_results.push_back(l_def);

		for (auto& obj : boxes) {
			Defect result;
			uint8_t b, g, r;
			tie(b, g, r) = iLogger::random_color(obj.class_label);
			cv::rectangle(image, cv::Point(obj.left, obj.top), cv::Point(obj.right, obj.bottom), cv::Scalar(b, g, r), 5);
			result.cls = obj.class_label;
			result.rect = cv::Rect(obj.left, obj.top, obj.right- obj.left, obj.bottom- obj.top);
			image_results.push_back(result);
			auto name = cocolabels[obj.class_label];
			auto caption = iLogger::format("%s %.2f", name, obj.confidence);
			int width = cv::getTextSize(caption, 0, 1, 2, nullptr).width + 10;
			cv::rectangle(image, cv::Point(obj.left - 3, obj.top - 33), cv::Point(obj.left + width, obj.top), cv::Scalar(b, g, r), -1);
			cv::putText(image, caption, cv::Point(obj.left, obj.top - 5), 0, 1, cv::Scalar::all(0), 2, 16);
		}
		batch_results.push_back(image_results);
		if (MODE_DEBUG) {
			string name = to_string(i);
			string save_path = iLogger::format("%s/%s.jpg", root.c_str(), name.c_str());
			INFO("Save to %s, %d object, average time %.2f ms", save_path.c_str(), boxes.size(), inference_average_time);
			cv::imwrite(save_path, image);
		}
	}
	return batch_results;
}

void readFileJson(string config_file)
{
	Json::Reader reader;
	Json::Value root;

	//从文件中读取，保证当前文件有demo.json文件  
	ifstream in(config_file, ios::binary);

	if (!in.is_open())
	{
		cout << "Error opening file\n";
		return;
	}

	if (reader.parse(in, root))
	{
		//读取根节点信息  
		MODEL_NAME = root["MODEL_NAME"].asString();
		INFER_TYPE = root["INFER_TYPE"].asString();
		DEVICE_ID = root["DEVICE_ID"].asInt();
		BATCH_SIZE = root["BATCH_SIZE"].asInt();
		MODE_DEBUG = root["MODE_DEBUG"].asBool();
		OBJ_THRESHOLD = root["OBJ_THRESHOLD"].asFloat();
		NMS_THRESHOLD = root["NMS_THRESHOLD"].asFloat();
		WORK_SPACE = root["WORK_SPACE"].asString();

		cout << "Reading Complete!" << endl;
	}
	else
	{
		cout << "parse error\n" << endl;
	}

	in.close();
}



static void create_engine_file(Yolo::Type type, TRT::Mode mode, const string& model, string onnx_file, string model_file, int batch_size) {

	int deviceid = DEVICE_ID;
	auto mode_name = TRT::mode_string(mode);
	TRT::set_device(deviceid);

	auto int8process = [=](int current, int count, const vector<string>& files, shared_ptr<TRT::Tensor>& tensor) {

		INFO("Int8 %d / %d", current, count);

		for (int i = 0; i < files.size(); ++i) {
			auto image = cv::imread(files[i]);
			cv::Mat temp_img = image;
			Yolo::image_to_tensor(image, tensor, type, i);
		}
	};

	INFO("===================== run  %s %s ==================================", Yolo::type_name(type), mode_name);


	if (not iLogger::exists(model_file)) {
		TRT::compile(
			mode,                       // FP32、FP16、INT8
			batch_size,            // max batch size
			onnx_file,                  // source 
			model_file,                 // save to
			{},
			int8process,
			"inference"
		);
	}


}


shared_ptr<Yolo::Infer> init_yolo(string config_file) {
    /***************   default params    *********************/


	readFileJson(config_file);


	/*****************    create engine   ************************/

	int batch_size = BATCH_SIZE;
	float obj_threshold = OBJ_THRESHOLD;
	float nms_threshold = NMS_THRESHOLD;
	string onnx_file = WORK_SPACE + MODEL_NAME+ ".onnx";
	string engine_file = WORK_SPACE + MODEL_NAME + "." + INFER_TYPE +".trtmodel";
	assert(INFER_TYPE == "FP32" || INFER_TYPE == "FP16");

	if (INFER_TYPE == "FP32")
	{
		create_engine_file(Yolo::Type::V5, TRT::Mode::FP32, "yolov5s", onnx_file, engine_file, batch_size);
	}
	else if (INFER_TYPE == "FP16")
	{
		create_engine_file(Yolo::Type::V5, TRT::Mode::FP16, "yolov5s", onnx_file, engine_file, batch_size);
	}


	auto engine = Yolo::create_infer(engine_file, Yolo::Type::V5, DEVICE_ID, obj_threshold, nms_threshold);
	if (engine == nullptr) {
		INFOE("Engine is nullptr");
		return nullptr;
	}


	// warmup
	auto files = iLogger::find_files(WORK_SPACE + "warmup", "*.jpg;*.jpeg;*.png;*.bmp;*.tif");
	vector<cv::Mat> images;
	for (int i = 0; i < files.size(); ++i) {
		auto image = cv::imread(files[i]);
		images.emplace_back(image);
	}
	vector<shared_future<Yolo::BoxArray>> boxes_array;
	for (int i = 0; i < 100; ++i)
		boxes_array = engine->commits(images);
	boxes_array.back().get();
	boxes_array.clear();
	return engine;
}