#pragma once

#include <opencv2/opencv.hpp>
#include "defectInfos.h"

enum DefectFlag {
    DEFECT_NON=0,     //  0     //non-defect
    DEFECT_POINT_L=1,// 1     //   large point defect 
    DEFECT_POINT_S=2,//2     //  smalll point defect 
    DEFECT_WRAP_L=3,//3     //  long wrap  defect
    DEFECT_WRAP_S=4,//4     // short wrap  defect 
    DEFECT_WEFT_L=5,//5     // long weft defect
    DEFECT_WEFT_S=6,//6     // short weft defect
    DEFECT_WEFT_D=7//7     // weft density defect
};


typedef struct DefectStruct {
    std::string img_name = "";
    int  idx_image;                              // 所属图像id

    cv::Size size_img = cv::Size(0, 0);           // 所属图像尺寸
               //缺陷类别
    cv::Rect rect = cv::Rect(0, 0, 0, 0);
    // 缺陷最小外接矩形
    int category = DEFECT_NON;
    double cls = 0;
    double area = -1;                             //  缺陷面积
    double maximum = -1;                           //  最大异常值
    double score = -1;                             //  缺陷评分
    cv::Mat mask;          //缺陷掩码
    cv::String info = "";

} Defect;
typedef  std::vector<Defect> Defects;