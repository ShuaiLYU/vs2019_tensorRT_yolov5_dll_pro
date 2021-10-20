#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include<queue>

#include "FDAlgorithmImplement.h"


FDAlgorithmImplement::FDAlgorithmImplement()
{


}
FDAlgorithmImplement::FDAlgorithmImplement(std::string  workspace)
{

	this->p_engine = init_yolo(workspace);

}


FDAlgorithmImplement::~FDAlgorithmImplement()
{
	//p_engine = NULL;
	cout << " destroy the FDAlgorithmImplement obj" << endl;

}

 void FDAlgorithmImplement::forward(const std::vector<cv::Mat>& imgs,std::vector<Defects>& batch_results)

{

	 this->m_defects.clear();

	 this->m_defects =inference_and_performance(this->p_engine, imgs);

	 //for (int i = 0; i < results.size(); i++)
	 //{
		// Defects* p_dll_storage = &results[i];
		// Defects* p_host_storage = &batch_results[i];
		// //p_host_storage->resize(p_dll_storage->size());

		// for (int j = 0; j < p_dll_storage->size(); j++)
		// {
		//	 float* rect = new float[4];
		//
		//	 rect[0] = p_dll_storage->at(j).rect.x;
		//	 rect[1] = p_dll_storage->at(j).rect.y;
		//	 rect[2] = p_dll_storage->at(j).rect.width;
		//	 rect[3] = p_dll_storage->at(j).rect.height;
		//	 int* cls = new int;
		//	 *cls = p_dll_storage->at(j).cls;

		//	 Defect l_def;
		//	 l_def.rect = cv::Rect(rect[0], rect[1], rect[2], rect[3]);
		//	 l_def.cls = *cls;
		//	 delete[] rect;
		//	 delete cls;

		//	 //Defect l_def;
		//	 //l_def.rect = p_dll_storage->at(j).rect;
		//	 //l_def.cls = p_dll_storage->at(j).cls;

		//	 p_host_storage->at(j) = l_def;
		// }

	 //}
	 return;

}






