// DigitRecDoc.cpp : implementation of the CDigitRecDoc class
//

#include "stdafx.h"
#include "DigitRec.h"

#include "DigitRecDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDigitRecDoc

IMPLEMENT_DYNCREATE(CDigitRecDoc, CDocument)

BEGIN_MESSAGE_MAP(CDigitRecDoc, CDocument)
	//{{AFX_MSG_MAP(CDigitRecDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDigitRecDoc construction/destruction

CDigitRecDoc::CDigitRecDoc()
{
	// TODO: add one-time construction code here

}

CDigitRecDoc::~CDigitRecDoc()
{

}

BOOL CDigitRecDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDigitRecDoc serialization

void CDigitRecDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDigitRecDoc diagnostics

#ifdef _DEBUG
void CDigitRecDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDigitRecDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDigitRecDoc commands
