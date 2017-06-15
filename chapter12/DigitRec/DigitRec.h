// DigitRec.h : main header file for the DIGITREC application
//

#if !defined(AFX_DIGITREC_H__49E1289E_FD3E_463F_AE4F_2A26C136B1E9__INCLUDED_)
#define AFX_DIGITREC_H__49E1289E_FD3E_463F_AE4F_2A26C136B1E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


#define IMG_HEIGHT 64
#define IMG_WIDTH 32

/////////////////////////////////////////////////////////////////////////////
// CDigitRecApp:
// See DigitRec.cpp for the implementation of this class
//

class CDigitRecApp : public CWinApp
{
public:
	CDigitRecApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigitRecApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDigitRecApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITREC_H__49E1289E_FD3E_463F_AE4F_2A26C136B1E9__INCLUDED_)
