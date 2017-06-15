// DigitRecView.h : interface of the CDigitRecView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIGITRECVIEW_H__04F4AB0F_60C6_40E2_BDCD_BF4572D377F0__INCLUDED_)
#define AFX_DIGITRECVIEW_H__04F4AB0F_60C6_40E2_BDCD_BF4572D377F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000






#define NUM_TRAINING 1000 //最多允许的训练样本数目


class CDigitRecView : public CScrollView
{
protected: // create from serialization only
	CDigitRecView();
	DECLARE_DYNCREATE(CDigitRecView)

// Attributes
public:
	CDigitRecDoc* GetDocument();

	vector<RECT> m_vecRT; //预处理时分割字符所要用到的矩形向量数组

	bool m_bOut; //切换 m_ORCImg 和 m_OutOCRImg 的显示
	COCRImageProcess m_OCRImg;
	COCRImageProcess m_OutOCRImg;

	double m_aryTrainingTag[NUM_TRAINING]; //训练样本的对应类标号(可能为实数，如函数拟合)
// Operations
public:
	BOOL ReadClassLabels(CString strFilePath); //读入训练样本类别信息
	void SaveToTrainDIR(CString strTrainDIR); //将预处理结果保存至训练样本分类目录
	void SaveToTestDIR(CString strTestDIR); //将预处理结果保存至测试样本分类目录
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigitRecView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDigitRecView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDigitRecView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTrain();
	afx_msg void OnRec();
	afx_msg void OnPreprocess();
	afx_msg void OnFileOpen();
	afx_msg void OnUpdatePreprocess(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileRotate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DigitRecView.cpp
inline CDigitRecDoc* CDigitRecView::GetDocument()
   { return (CDigitRecDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITRECVIEW_H__04F4AB0F_60C6_40E2_BDCD_BF4572D377F0__INCLUDED_)
