#include "StdAfx.h"
#include "Tools.h"

CTools::CTools(void)
{
}


CTools::~CTools(void)
{
}


float CTools::DrawToCDC(IplImage* img,CDC* pDC)
{
	CRect rect;
	CImage bkgound;
	float ratio;
	BITMAPINFO bmi;
	BITMAPINFOHEADER* bmih = &(bmi.bmiHeader);
	memset( bmih, 0, sizeof(*bmih));
	IplImage* pImg = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,3);

	if ((img->nChannels * img->depth) == 8)
	{
		cvCvtColor(img,pImg,CV_GRAY2RGB);
	}

	if ((img->nChannels * img->depth) == 24)
	{
		cvCopy(img,pImg);
	}
	if ((img->nChannels * img->depth) > 24)
	{
		AfxMessageBox(L"ÇëÊäÈë8BPP»ò24BPPµÄÍ¼Ïñ£¡");
		return 0;
	}

	bmih->biSize   = sizeof(BITMAPINFOHEADER);
	bmih->biWidth  = pImg->width;
	bmih->biHeight = -abs(pImg->height);
	bmih->biPlanes = 1;
	bmih->biBitCount = 24;
	bmih->biCompression = BI_RGB;

	pDC->GetWindow()->GetWindowRect(&rect);
	bkgound.Create(rect.Width(),rect.Height(),24);
	if((float)rect.Height()/(pImg->height) > (float)rect.Width()/(pImg->width))
	{
		ratio = (float)rect.Width()/(pImg->width);
		bkgound.Draw(pDC->GetSafeHdc(),0,(int)((pImg->height)*ratio),rect.Width(),rect.Height() - (int)((pImg->height)*ratio));
	}
	else{
		ratio = (float)rect.Height()/(pImg->height);
		bkgound.Draw(pDC->GetSafeHdc(),(int)((pImg->width)*ratio),0,rect.Width() - (int)((pImg->width)*ratio),rect.Height());
	}	
	pDC->SetStretchBltMode(HALFTONE);
	::StretchDIBits(pDC->GetSafeHdc(),0,0,(int)((pImg->width)*ratio),(int)((pImg->height)*ratio),0,0,pImg->width,pImg->height,pImg->imageData,&bmi,DIB_RGB_COLORS,SRCCOPY);
	cvReleaseImage(&pImg);
	return ratio;
} 

CvPoint CTools::QuartzPostion(IplImage* src, IplImage* dst)
{
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * contour = 0;
	int mode = CV_RETR_EXTERNAL;
	double length;
	CvPoint2D32f center;
	float r;
	CvPoint pt; 

	pt.y = 1000;
	pt.x = 0;

	CalibrateData m_CalDat;

	GetCalirateParam(&m_CalDat);

	IplImage* temp = cvCreateImage(cvGetSize(src), 8, 1);
	cvCanny(src, temp, 50, 100);

	cvFindContours(temp, storage, &contour, sizeof(CvContour), mode);

	for( CvSeq* c = contour; c != NULL; c = c->h_next)
	{
		c = cvApproxPoly( c, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 5, 1 );
		length = cvArcLength(c, CV_WHOLE_SEQ, -1);
		if ((length > m_CalDat.WaferPxLow) && (length < m_CalDat.WaferPxHigh))
		{
			cvDrawContours(dst, c, CV_RGB(0,0,255), CV_RGB(255, 0, 0), -1, 2, 8);
			cvMinEnclosingCircle(c, &center, &r);
			if ((center.y > 336) && (center.y < pt.y))
			{
				pt = cvPointFrom32f(center);
			}
			//pt[num] = cvPointFrom32f(center);
			//cvCircle(pContoursImg, pt[num], 3, CV_RGB(0,0,255), -1);
			//cvCircle(pContoursImg, pt[num], r, CV_RGB(0,0,255), 2);
		}
	}
	cvCircle(dst, pt, 10, CV_RGB(255,0, 0), -1);
	cvReleaseImage(&temp);
	cvClearMemStorage( storage );
	cvReleaseMemStorage( &storage );

	return pt;

}

void CTools::GetCalirateParam(pCalibrateData pCalirateStruct)
{
	CFile file;
	if (file.Open(L"Calibrate.dat", CFile::modeRead))
	{
		file.Read(pCalirateStruct, sizeof(CalibrateData));
		file.Close();
	}
}