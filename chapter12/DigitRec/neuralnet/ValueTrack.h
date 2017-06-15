// ValueTrack.h: interface for the CValueTrack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUETRACK_H__FE95CD67_171A_48A6_B79E_812FE2C1B32D__INCLUDED_)
#define AFX_VALUETRACK_H__FE95CD67_171A_48A6_B79E_812FE2C1B32D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CValueTrack  
{
public:
	CValueTrack(CWnd *pWnd);
	virtual ~CValueTrack();

	void AddValue(double val);
	void Draw();
	void Init()
	{
		m_nValue = 0;
	}

	CWnd *m_pWnd; //绘图窗体
	CRect m_rt; //绘图区矩形

	int m_nMaxValues; //绘图区能容纳的最大误差值个数
	double *m_pValues; //误差值数组
	double *m_pTemp; //临时误差值数组
	int m_nValue; //误差值索引

private:
	// 绘制
	CBrush m_GroundBush;
	CBrush m_SelectBrush;
	CPen m_WavePen;
	CPen m_CoordinatePen;
	CPen m_ProcesslinePen;
	CPen m_LabelPen;
	CPen m_LabewavePen;
	CPen m_ProgramPen;

};

#endif // !defined(AFX_VALUETRACK_H__FE95CD67_171A_48A6_B79E_812FE2C1B32D__INCLUDED_)
