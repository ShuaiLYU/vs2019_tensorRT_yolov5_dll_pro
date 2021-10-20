#pragma once
#include "opencv2/opencv.hpp"
//#include "FabricDetectionDLL.h"
#include <app_yolo.hpp>

#include "algorithmInterface.h"


class FDAlgorithmImplement :
	public  FDAlgorithm
{


private:

	shared_ptr<Yolo::Infer> p_engine;
	
	


private:


	/****************intermediate variables ************************************/



public:


	FDAlgorithmImplement();
	FDAlgorithmImplement(std::string  workspace);

	virtual ~FDAlgorithmImplement();


	virtual void  forward(const std::vector<cv::Mat>& imgs, std::vector<Defects>& batch_results);

};

