// DigitRecDoc.h : interface of the CDigitRecDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIGITRECDOC_H__CF341AE9_99EB_443E_A0E0_6FB03423F61C__INCLUDED_)
#define AFX_DIGITRECDOC_H__CF341AE9_99EB_443E_A0E0_6FB03423F61C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDigitRecDoc : public CDocument
{
protected: // create from serialization only
	CDigitRecDoc();
	DECLARE_DYNCREATE(CDigitRecDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigitRecDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDigitRecDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDigitRecDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITRECDOC_H__CF341AE9_99EB_443E_A0E0_6FB03423F61C__INCLUDED_)
