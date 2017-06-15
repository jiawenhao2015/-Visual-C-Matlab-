#if !defined(AFX_TESTDIALOG_H__02B03DD6_8E81_4F13_99E0_ABE406A16966__INCLUDED_)
#define AFX_TESTDIALOG_H__02B03DD6_8E81_4F13_99E0_ABE406A16966__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestDialog.h : header file
//


#include "./OCRImageProcess/ocrImgProcess.h"
/////////////////////////////////////////////////////////////////////////////
// CTestDialog dialog

class CTestDialog : public CDialog
{
// Construction
public:
	CNeuralNet *m_pNet; //神经网络对象
	double  m_dHighestOutput; //最大输出单元的响应
	int m_nBestMatch;  //识别出的类别
	CRect m_rt; //图像处理的矩形区域
	BOOL m_bInTest; //是否在测试(识别)中
	COCRImageProcess m_Img; //待识别图像
	vector<CString> m_vecClassNames; //类别名称信息
	BOOL m_bReadyToRec; //是否可以进行识别

	CTestDialog(CWnd* pParent = NULL);   // standard constructor
	~CTestDialog();

	bool LoadFromFile(CString strFilePathName);
// Dialog Data
	//{{AFX_DATA(CTestDialog)
	enum { IDD = IDD_DIALOG_REC };
	CEdit	m_EditFile;
	CButton	m_ButtonTestBrowse;
	CEdit	m_EditLoadTrainFile;
	CButton	m_CheckDir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestDialog)
	afx_msg void OnButtonRec();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonTestBrowse();
	afx_msg void OnPaint();
	afx_msg void OnButtonStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDIALOG_H__02B03DD6_8E81_4F13_99E0_ABE406A16966__INCLUDED_)
