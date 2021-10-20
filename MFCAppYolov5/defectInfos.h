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
    int  idx_image;                              // ����ͼ��id

    cv::Size size_img = cv::Size(0, 0);           // ����ͼ��ߴ�
               //ȱ�����
    cv::Rect rect = cv::Rect(0, 0, 0, 0);
    // ȱ����С��Ӿ���
    int category = DEFECT_NON;
    double cls = 0;
    double area = -1;                             //  ȱ�����
    double maximum = -1;                           //  ����쳣ֵ
    double score = -1;                             //  ȱ������
    cv::Mat mask;          //ȱ������
    cv::String info = "";

} Defect;
typedef  std::vector<Defect> Defects;