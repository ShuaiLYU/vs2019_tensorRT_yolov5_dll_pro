#include "app_yolo.hpp"
#include <stdio.h>
#include <string.h>
#include <functional>


using namespace std;



int main(int argc, char** argv){


	string config_file= "F:\\baiduYUN\\tensorRT_yolov5_demo-v1\\workspace\\config.json";  //最后一定要有 \\

	auto files = iLogger::find_files("F:\\baiduYUN\\tensorRT_yolov5_demo-v1\\workspace\\samples", "*.jpg;*.jpeg;*.png;*.bmp;*.tif");
	vector<cv::Mat> images;
	for (int i = 0; i < files.size(); ++i) {
		auto image = cv::imread(files[i]);
		images.emplace_back(image);
	}


	auto engine = init_yolo(config_file);

	
	std::vector<Defects>result  = inference_and_performance(engine, images);

    return 0;
}


