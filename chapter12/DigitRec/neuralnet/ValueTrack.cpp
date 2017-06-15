// ValueTrack.cpp: implementation of the CValueTrack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\DigitRec.h"
#include "ValueTrack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//为误差跟踪显示预定义颜色
#define GROUND_RGB	RGB(0, 0, 0)
#define WAVEPEN_RGB RGB(0, 255, 0)
#define COPEN_RGB RGB(136, 136, 136)
#define PROCESSLINE_RGB RGB(0, 0, 255)
#define STARTLINE_RGB RGB(255, 255, 0)

#define SELECT_RGB RGB(255, 255, 255)

#define LABEL_RGB RGB(255, 0, 0)

#define LABELWAVE_RGB RGB(255, 127, 0)

#define STATION_RGB RGB(166, 202, 240)
#define PROGRAM_RGB RGB(0, 95, 63)


CValueTrack::CValueTrack(CWnd *pWnd)
{
	m_pWnd = pWnd;

	pWnd->GetWindowRect(&m_rt);
	
	m_nMaxValues = m_rt.Width() - 50;

	m_pValues = new double[m_nMaxValues];

	m_pTemp = new double[m_nMaxValues];

	m_nValue = 0;


	m_SelectBrush.CreateSolidBrush(SELECT_RGB);
	m_GroundBush.CreateSolidBrush(GROUND_RGB);
	m_WavePen.CreatePen(PS_SOLID, 1, WAVEPEN_RGB);
	m_CoordinatePen.CreatePen(PS_SOLID, 1, COPEN_RGB);
	m_ProcesslinePen.CreatePen(PS_SOLID, 1, PROCESSLINE_RGB);

	m_LabelPen.CreatePen(PS_SOLID, 1, LABEL_RGB);
	m_LabewavePen.CreatePen(PS_SOLID, 1, LABELWAVE_RGB);
	m_ProgramPen.CreatePen(PS_SOLID, 1, PROGRAM_RGB);


}

CValueTrack::~CValueTrack()
{
	delete []m_pTemp;
	delete []m_pValues;
}


void CValueTrack::Draw()
{
	CRect Rect;
	m_pWnd->GetClientRect(&Rect);
	CDC *pDC = m_pWnd->GetDC();
	
	int nBaseX = 25;
	int nWaveY = Rect.Height() - 10;

	HGDIOBJ pOldBrush = pDC->SelectObject(m_GroundBush);
	// 绘制黑色背景
	pDC->Rectangle(&Rect);

	HGDIOBJ pOldPen = pDC->SelectObject(m_CoordinatePen);

	// 绘制横向坐标轴
	pDC->MoveTo(nBaseX, nWaveY);
	pDC->LineTo(Rect.Width(), nWaveY);

	// 绘制纵向坐标轴
	pDC->MoveTo(nBaseX, nWaveY);
	pDC->LineTo(nBaseX, 0);	
	
	// 绘制时间刻度
	int nXOrg = 0;
	int nPixelsPerUnit = 50;
	
	// 绘制横轴刻度
	CString str;
	int nRng = Rect.Width();

	for(int i=nBaseX; i<=nRng; i++)
	{
		if((i-nBaseX) % nPixelsPerUnit == 0)
		{
			// 刻度
			pDC->MoveTo(i, nWaveY);
			pDC->LineTo(i, nWaveY - 5);
		}	
	}	

	//绘制纵轴刻度
	nRng = nWaveY;
	int nn = 0;
	double dRule = 0; //刻度值 0 0.001 0.01 0.1 1
	int ySpan = nRng/4 - 5; //计算纵轴刻度间长度
	pDC->SetBkColor(GROUND_RGB);
	pDC->SetTextColor(COPEN_RGB);
	for(i=nRng; i>=0; i--, nn++)
	{
		if((nn) % ySpan == 0)
		{
			// 刻度
			pDC->MoveTo(nBaseX, i);
			pDC->LineTo(nBaseX + 7, i);	
			if(dRule == 0.001)
				str.Format("%.3f",dRule);
			else if (dRule == 0.01)
				str.Format("%.2f",dRule);
			else
				str.Format("%.1f",dRule);
			if(dRule == 0)
				dRule = 0.001;
			else 
				dRule *= 10;
			
			pDC->TextOut(nBaseX - 25, i-8, str);
		}	
	}	

	
	/////////// 绘制误差点//////////
	pDC->SelectObject(m_WavePen);
	if(m_nValue > 0)
		pDC->MoveTo(nBaseX, nRng - 3*ySpan);
	for(i=0; i<m_nValue; i++)
	{		
		if(m_pValues[i] <= 0.001) // 0.0001 - 0.001
		{
			dRule = m_pValues[i]/(0.001/ySpan);
			pDC->SetPixel(i+nBaseX, nRng - dRule, WAVEPEN_RGB);
		}
		else if(m_pValues[i] <= 0.01) // 0.001 - 0.01	
		{
			dRule = m_pValues[i]/(0.01/ySpan);
			pDC->SetPixel(i+nBaseX, nRng - (dRule + ySpan), WAVEPEN_RGB);
		}
		else  if(m_pValues[i] <= 0.1)// 0.01 - 0.1
		{
			dRule = m_pValues[i]/(0.1/ySpan);
			pDC->SetPixel(i+nBaseX, nRng - (dRule + ySpan*2), WAVEPEN_RGB);
		}
		else  if(m_pValues[i] <= 1)// 0.1 - 1
		{
			dRule = m_pValues[i]/(1.0/ySpan);
			pDC->SetPixel(i+nBaseX, nRng - (dRule + ySpan*3), WAVEPEN_RGB);
		}
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	m_pWnd->ReleaseDC(pDC);
}

void CValueTrack::AddValue(double val)
{
	if(m_nValue >= m_nMaxValues) //当前绘图区已满
	{
		// 整体前移1位
		memcpy(m_pTemp, m_pValues + 1, sizeof(double)*(m_nMaxValues-1));

		m_pTemp[m_nMaxValues-1] = val; //当前误差放入最后位置

		memcpy(m_pValues, m_pTemp, sizeof(double)*(m_nMaxValues));
	}
	else
	{
		m_pValues[m_nValue++] = val;//加入当前误差
	}
}