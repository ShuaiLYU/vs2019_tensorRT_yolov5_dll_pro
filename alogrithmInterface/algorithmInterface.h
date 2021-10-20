#pragma once
#include "opencv2/opencv.hpp"
#include <vector>
using namespace std;
/*********************
@author : Lyu Shuai
@Date:  08/12/2020
@Dependencies:  opencv3.1. 0 ,cuda8.0
@development environment:  visual studio  2019

*************************************/







#ifdef MATUREAPPROACH_EXPORTS
#define MATUREAPPROACH_API __declspec(dllexport)
#else
#define MATUREAPPROACH_API __declspec(dllimport)
#pragma comment(lib, "../x64/Debug/FabricDetectionAlgorithm.lib")
#endif

#include "defectInfos.h"



class  FDAlgorithm
{


	

public:


	/*******************************************************************/

	FDAlgorithm();


	virtual ~FDAlgorithm() {};

	virtual void forward(const std::vector<cv::Mat>& imgs, std::vector<Defects>& batch_results) =0;

	std::vector<Defects> m_defects;

};


extern "C" MATUREAPPROACH_API FDAlgorithm * _stdcall CreateExportObj(std::string  workspace);

