#pragma once

#include "cv.h"
#include "highgui.h"

typedef struct CalibrateStruct{
	UINT PlatPulseNum;
	CvPoint2D64d OriginPoint;
	double Param_k;
	double Param_b;
	double InnerRadius;
	double OuterRadius;
	UINT WaferPxHigh;
	UINT WaferPxLow;
}CalibrateData, *pCalibrateData;


class CTools
{
public:
	CTools(void);
	~CTools(void);

	float DrawToCDC(IplImage* img,CDC* pDC);
	CvPoint QuartzPostion(IplImage* src, IplImage* dst);
	void GetCalirateParam(pCalibrateData pCalirateStruct);
	
};

