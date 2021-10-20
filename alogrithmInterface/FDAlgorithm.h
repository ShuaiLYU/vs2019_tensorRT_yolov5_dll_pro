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




//#define MATUREAPPROACH_EXPORTS


#ifdef MATUREAPPROACH_EXPORTS
#define MATUREAPPROACH_API __declspec(dllexport)
#else
#define MATUREAPPROACH_API __declspec(dllimport)
#pragma comment(lib, "../x64/Debug/FabricDetectionAlgorithm.lib")
#endif





enum DefectFlag {
	DEFECT_NON,     //  0     //non-defect
	DEFECT_POINT_L,// 1     //   large point defect 
	DEFECT_POINT_S,//2     //  smalll point defect 
	DEFECT_WRAP_L,//3     //  long wrap  defect
	DEFECT_WRAP_S,//4     // short wrap  defect 
	DEFECT_WEFT_L,//5     // long weft defect
	DEFECT_WEFT_S,//6     // short weft defect
	DEFECT_WEFT_D//7     // weft density defect
};
typedef struct DefectStruct {
	int  idx_image;                              // 所属图像id
	cv::Size size_img = cv::Size(0, 0);           // 所属图像尺寸
	DefectFlag flag = DEFECT_NON;                //缺陷类别
	cv::Rect rect = cv::Rect(0, 0, 0, 0);        // 缺陷最小外接矩形
	double area = -1;                             //  缺陷面积
	double maximum = -1;                           //  最大异常值
	double score = -1;                             //  缺陷评分
	cv::Mat mask;										//缺陷掩码
	cv::String info = "";

} Defect;
typedef  std::vector<Defect> Defects;


class  FDAlgorithm
{

private:



public:

	cv::String fabric_type = "";
	cv::String img_name = "";
	int image_idx;      // = img_info.image_idx;
	cv::Mat ImgRgb;  // RBG input  image
	cv::Mat ImgGray;  // Gray image
	cv::Mat ImgZoom;    //  zoom  the image size 
	cv::Mat ImgEven;   // 
	cv::Mat ImgFourier;
	cv::Mat mask_babric;
	Defects defects_point;
	Defects defects_blot;
	Defects defects_wrap_line;
	Defects defects_weft_line;
	Defects defects_density_weft;
	double num_wefts = 0; //图片中有多少纬线
	cv::Mat img_wefts,weft_histtogram;
	int edge_status, point_status, line_status, weft_status, blot_status;  // 检测结果
	bool b_backLight;
	double zoom_factor;
	cv::Point pt_center;




	/*********************control varivables********************************/
	bool use_GPU;
	bool b_debug;
	/********************************xml variables**************************************/
	int img_width=512;
	int method_perprocessing;
	/**********Fabric Edge Parameters**********/
	int fabric_edge_thresh = 15;
	int	fabric_edge_kernel_size = 50;

	/**********Point Defect Parameters**********/
	double point_thresh_value = 400;
	double point_thresh_volume = 3;
	double point_area_min = 30;
	/**********Gabor Defect Parameters**********/
	int orientation_num, scale_num;
	int length_of_count;
	/**********Line Defect Parameters**********/
	double line_area_thres = 20;
	double line_val_thres = 2;
	double line_score_thres = 50;
	/**********weft Defect Parameters**********/
	int m_weft_bin_num = 9;
	int m_weft_range_thres = 6;
	int m_weft_bin_thres = 10;
	int m_weft_roi_width = 16; //16

	/*******************************************************************/

	FDAlgorithm();
	virtual ~FDAlgorithm() {};
	/**************************
	* @Func:  Set the input of  the algorithm object
	* @param: srcImage - BGR  input image 
	* @param: fabric_type -  fabric type,  options: ["P","D"]
	* @param b_back_light -  whether is back light. options: [True, False]
	* @optional param:  img_name -  the file name of input name
	*******************************/
	virtual int setInput(cv::Mat srcImage, cv::String fabric_type, bool b_back_light, cv::String img_name = "")=0;
	/**************************
	* @Func:  Determine whether it is the edge of the fabric
	* @optional param:  pEdge -  Not yet used.
	*******************************/
	virtual int is_fabricEdge(int pEdge) = 0;
	/**************************
	* @Func:  perform point defect detection
	*******************************/
	virtual int detectedPointDefects() = 0;
	/**************************
	//* @Func:  perform blot defect detection
	//*******************************/
	//virtual int detectedBlotDefects() = 0;
	/**************************
	* @Func:  perform weft density detection
	*******************************/
	virtual int detectedWeftDensityDefects() = 0;
	/**************************
	* @Func:  perform line defect detection
	*******************************/
	virtual int detectedLineDefects()=0;

	/**************************
	 * @Func:  return the defects
	*******************************/
	virtual  Defects returnPointDefects() = 0;
	virtual Defects returnWeftLineDefects() = 0;
	virtual Defects returnWrapLineDefects() = 0;
	virtual Defects returnWeftDensityDefects() = 0;
	virtual cv::Mat returnWeftImage() = 0;
	virtual int returnNumOfWefts()=0;
	/**************************
	* @Func:  clear varibles of the algorithm object
	*******************************/
	virtual void reSet() = 0;
	/**************************
	* @Func:   learn parameter (disabled)
	*******************************/
	virtual int learnParameters() = 0;

	virtual void initParameters(const std::vector<double>& params)=0;
	virtual void log()=0;
	virtual void setLogDir(cv::String log_dir)=0;


	virtual int  getDefectsOnBinaryImg(const cv::Mat& img_mask, cv::Mat& img_defect, Defects& defects, const int area_thres)=0;
	virtual void logDefects(const cv::Mat& img, const Defects& defects, cv::String info) = 0;
	virtual void imWrite(const cv::String& img_name, const cv::Mat& img)=0;
	virtual void logInfo(const cv::String& info)=0;
public:
	

	/*******************************************
	@Author: Lyu Shuai
	@Date:  10/12/2020
	@Param:  defectCategory:  { 0: all categories (所有种类) 1：point defects  2:  wrap defects  3: weft defects }
	@Func:   display
	@Return:   a image with detected defects
	*******************************************/
	//Mat displayDefectsOnImg(int defectCategory = 0);
};


extern "C" MATUREAPPROACH_API FDAlgorithm * _stdcall CreateExportObj(bool use_GPU, bool b_debug);

