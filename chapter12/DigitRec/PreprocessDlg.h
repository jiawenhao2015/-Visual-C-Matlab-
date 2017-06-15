#if !defined(AFX_PREPROCESSDLG_H__D1C46F24_ADE0_47C1_BE6D_08FA388EBB13__INCLUDED_)
#define AFX_PREPROCESSDLG_H__D1C46F24_ADE0_47C1_BE6D_08FA388EBB13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreprocessDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreprocessDlg dialog

class CPreprocessDlg : public CDialog
{
// Construction
public:
	CPreprocessDlg(CWnd* pParent = NULL);   // standard constructor
	bool bTrain; //是否是训练集合的预处理
// Dialog Data
	//{{AFX_DATA(CPreprocessDlg)
	enum { IDD = IDD_DIALOG_PREPROCESS };
	UINT	m_nThres;
	UINT	m_nUpper;
	UINT	m_nLower;
	CString	m_strTrainFile;
	CString	m_strClassLabel;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreprocessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreprocessDlg)
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonDir();
	afx_msg void OnRadioTest();
	afx_msg void OnRadioTrain();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREPROCESSDLG_H__D1C46F24_ADE0_47C1_BE6D_08FA388EBB13__INCLUDED_)
