// OCRImgProcess.h: interface for the COCRImageProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OCRIMAGEPROCESS_H__5E3CECEC_A041_49A1_9C04_531E0179083F__INCLUDED_)
#define AFX_OCRIMAGEPROCESS_H__5E3CECEC_A041_49A1_9C04_531E0179083F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgProcess.h"



//从通用图像处理类 CImgProcess 派生，专门负责数字识别系统图像预处理的类
class COCRImageProcess  : public CImgProcess
{
public:
	COCRImageProcess();
	virtual ~COCRImageProcess();

	COCRImageProcess& operator = (CImg& img);

	void DelScatterNoise(COCRImageProcess* pTo, int upperThres, int lowerThres);//去除离散噪声

	//几何变换
	void SlopeAdjust(COCRImageProcess* pTo); //目标对象（字符）整体倾斜度调整
	vector<RECT> ObjectSegment(); //对象（字符）分割
	void ObjectNorm(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, vector<RECT> &vecRT); //对象（字符）大小归一化
	void ObjectAlign(COCRImageProcess* pTo, vector<RECT> &vecRT); //目标对象（字符）的紧缩对齐


	//点运算
	RECT RgnZoom(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, LPRECT lpRect);//图像中某个区域的缩放

};

#endif // !defined(AFX_OCRIMAGEPROCESS_H__5E3CECEC_A041_49A1_9C04_531E0179083F__INCLUDED_)
