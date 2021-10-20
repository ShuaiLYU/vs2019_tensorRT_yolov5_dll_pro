#ifndef APP_YOLO_HPP
#define APP_YOLO_HPP


#include "app_yolo/yolo.hpp"
//#include "yaml-cpp/yaml.h"
#include <memory>

#include <builder/trt_builder.hpp>
#include <infer/trt_infer.hpp>
#include <common/ilogger.hpp>
#include <common/json.hpp>

#include <alogrithmInterface/defectInfos.h>
using namespace std;





shared_ptr<Yolo::Infer> init_yolo(string config_file);
std::vector<Defects> inference_and_performance(shared_ptr<Yolo::Infer>engine, vector<cv::Mat> images);
#endif // YOLO_HPP