// DigitRecView.cpp : implementation of the CDigitRecView class
//

#include "stdafx.h"
#include "DigitRec.h"

#include "DigitRecDoc.h"
#include "DigitRecView.h"
#include "TrainDlg.h"
#include "TestDialog.h"
#include "preprocessDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDigitRecView

IMPLEMENT_DYNCREATE(CDigitRecView, CScrollView)

BEGIN_MESSAGE_MAP(CDigitRecView, CScrollView)
	//{{AFX_MSG_MAP(CDigitRecView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDM_TRAIN, OnTrain)
	ON_COMMAND(IDM_REC, OnRec)
	ON_COMMAND(IDM_PREPROCESS, OnPreprocess)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(IDM_PREPROCESS, OnUpdatePreprocess)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_ROTATE, OnFileRotate)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()


//返回指定文件夹下的bmp图像文件数目
int GetFileNum(CString strDir)
{
	int nBMPFile = 0;

	CString strToFind = strDir; 
	strToFind += "*.bmp";
		
	WIN32_FIND_DATA findData;
	HANDLE hFindFile;
	
	hFindFile = ::FindFirstFile(strToFind, &findData);

	if(hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(findData.cFileName[0] == '.')
				continue;
				
			if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				nBMPFile++;	//找到1个BMP文件	
		}
		while(::FindNextFile(hFindFile, &findData));
			
		::FindClose(hFindFile);
	}
	else
		return 0;

	return nBMPFile;
}	






/////////////////////////////////////////////////////////////////////////////
// CDigitRecView construction/destruction
/**********************
CDigitRecView::CDigitRecView()
构造函数
**********************/
CDigitRecView::CDigitRecView()
{
	for(int i=0; i<NUM_TRAINING; i++)
	{
		m_aryTrainingTag[i] = i % 10;
	}

}

CDigitRecView::~CDigitRecView()
{
}

BOOL CDigitRecView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	


	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDigitRecView drawing

void CDigitRecView::OnDraw(CDC* pDC)
{
	CDigitRecDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if(m_bOut)
		{
			m_OutOCRImg.Draw(pDC);

			for(int i=0; i<m_vecRT.size(); i++)
			{
				RECT rt = m_vecRT[i];

				
				pDC->MoveTo(rt.left, rt.top);
				pDC->LineTo(rt.right, rt.top);
				
				pDC->MoveTo(rt.right, rt.top);
				pDC->LineTo(rt.right, rt.bottom);

				pDC->MoveTo(rt.right, rt.bottom);
				pDC->LineTo(rt.left, rt.bottom);

				pDC->MoveTo(rt.left, rt.bottom);
				pDC->LineTo(rt.left, rt.top);

			}
			
		}
		else
			m_OCRImg.Draw(pDC);


		
	
}

/////////////////////////////////////////////////////////////////////////////
// CDigitRecView printing

BOOL CDigitRecView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDigitRecView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDigitRecView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDigitRecView diagnostics

#ifdef _DEBUG
void CDigitRecView::AssertValid() const
{
	CView::AssertValid();
}

void CDigitRecView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDigitRecDoc* CDigitRecView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDigitRecDoc)));
	return (CDigitRecDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDigitRecView message handlers

void CDigitRecView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	
	if(m_OCRImg.IsValidate())
	{
		sizeTotal.cx = m_OCRImg.GetWidthPixel();
		sizeTotal.cy = m_OCRImg.GetHeight();
	}
	SetScrollSizes(MM_TEXT, sizeTotal);

}

void CDigitRecView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default


	CView::OnLButtonDown(nFlags, point);
}

void CDigitRecView::OnTrain() 
{
	//弹出预设定对话框，请用户输入一些必要的训练配置信息
	CTrainDlg trainDlg;
	trainDlg.DoModal();	
}

void CDigitRecView::OnRec() 
{
	CTestDialog testDlg;
	testDlg.DoModal(); //弹出测试对话框
}

BOOL CDigitRecView::ReadClassLabels(CString strFilePath)
{
	FILE *fp;
	fp = fopen(strFilePath, "r");	
	if(fp == NULL)
	{
		AfxMessageBox("无法打开指定的文件！");
		return FALSE;
	}

	int i = 0;
	char ch;
	while((ch=fgetc(fp))!=EOF) //读入字符直到文件结束
	{
		if(ch == ' ') //跳过空格
			continue;
		m_aryTrainingTag[i] = ch-48; //转化为整型类标签
		i++;
	}

	fclose(fp);

	return TRUE;
}

void CDigitRecView::SaveToTrainDIR(CString strTrainDIR)
{
	CString strFilePath = strTrainDIR; //训练样本目录
	CreateDirectory(strFilePath, NULL); 
	int nFileIndex; //指出是该类的第几个文件
	int i, j;
	int iNew, jNew;

	//将训练样本分门别类存入训练样本目录
	int nSize = m_vecRT.size();
	for(int nRT=0; nRT<nSize; nRT++)
	{
		strFilePath = strTrainDIR;
		RECT rt = m_vecRT[nRT];

		int nHeight = (rt.bottom-1) - (rt.top+1) + 1;
		int nWidth = (rt.right-1) - (rt.left+1) + 1;

		COCRImageProcess imgToWrite;
		imgToWrite = m_OCRImg;//分配空间
		imgToWrite.ImResize(nHeight, nWidth); //重新分配空间

		// 拷贝矩形区域中的像素到图像 img 中
		iNew = 0;
		jNew = 0;
		for(i=rt.top+1; i<rt.bottom; i++) //矩形在建立时膨胀了一个像素，取像素时内缩一个像素
		{
			jNew = 0;
			for(j=rt.left+1; j<rt.right; j++)
			{
				int nGray = m_OutOCRImg.GetGray(j, i);
				imgToWrite.SetPixel( jNew, iNew, RGB(nGray, nGray, nGray) );
				jNew ++;
			}// for j

			iNew ++;
		}// for i
		

		int nClass = m_aryTrainingTag[nRT];
		CString strDIR; //类别目录
		strDIR.Format("\\%d\\", nClass);
		strFilePath += strDIR;

		//如果没有类别目录，首先建立之
		CreateDirectory(strFilePath, NULL);

		//同一类别目录下文件按照建立顺序编号
		nFileIndex = GetFileNum(strFilePath);
		CString strFile; //文件名
		strFile.Format("%d.bmp", nFileIndex);
		strFilePath += strFile;

		// 将预处理后的图像写入训练分类目录
		imgToWrite.SaveToFile(strFilePath);

	}// for nRT
}

void CDigitRecView::SaveToTestDIR(CString strTestDIR)
{
	CString strFilePath = strTestDIR+"\\"; //测试样本目录
	CreateDirectory(strFilePath, NULL); 
	int nFileIndex; //指出是该类的第几个文件
	int i, j;
	int iNew, jNew;

	//将测试样本存入测试样本目录
	int nSize = m_vecRT.size();
	for(int nRT=0; nRT<nSize; nRT++)
	{
		strFilePath = strTestDIR+"\\";;
		RECT rt = m_vecRT[nRT];

		int nHeight = (rt.bottom-1) - (rt.top+1) + 1;
		int nWidth = (rt.right-1) - (rt.left+1) + 1;

		COCRImageProcess imgToWrite;
		imgToWrite = m_OCRImg;//分配空间
		imgToWrite.ImResize(nHeight, nWidth); //重新分配空间

		// 拷贝矩形区域中的像素到图像 img 中
		iNew = 0;
		jNew = 0;
		for(i=rt.top+1; i<rt.bottom; i++) //矩形在建立时膨胀了一个像素，取像素时内缩一个像素
		{
			jNew = 0;
			for(j=rt.left+1; j<rt.right; j++)
			{
				int nGray = m_OutOCRImg.GetGray(j, i);
				imgToWrite.SetPixel( jNew, iNew, RGB(nGray, nGray, nGray) );
				jNew ++;
			}// for j

			iNew ++;
		}// for i
		
		//如果没有类别目录，首先建立之
		CreateDirectory(strFilePath, NULL);

		//目录下文件按照建立顺序编号
		nFileIndex = GetFileNum(strFilePath);
		CString strFile; //文件名
		strFile.Format("%d.bmp", nFileIndex);
		strFilePath += strFile;

		// 将预处理后的图像写入测试样本目录
		imgToWrite.SaveToFile(strFilePath);

	}// for nRT
}


void CDigitRecView::OnPreprocess() 
{
	//取得预处理相关的参数
	int nThres; //二值化的阈值
	int nUpper, nLower; //去除离散噪声时的上、下限（大于nUpper或小于nLower的被作为噪声滤除）
	CString strLabelFilePath; // 类别标签文件的路径信息
	CString strDIR; //保存训练样本的分类目录
	bool bTrain; //是否是训练集合的预处理

	//设置默认值
	nThres = 145;
	nLower = 15;
	nUpper = 1500;

	CPreprocessDlg dlgPara;
	if( dlgPara.DoModal() != IDOK)
		return;
	else
	{//从对话框中取得用户的设置
		nThres = dlgPara.m_nThres; //二值化阈值
		nUpper = dlgPara.m_nUpper; //去除噪声时的连通区域上限
		nLower = dlgPara.m_nLower; //去除噪声时的连通区域下限
		strLabelFilePath = dlgPara.m_strClassLabel; //训练集样本的类标签文件
		strDIR = dlgPara.m_strTrainFile; //训练或测试集的目录
		bTrain = dlgPara.bTrain; //训练模式还是测试模式
	}

	// 从类别标签文件读取类别标签信息
	if(bTrain)
		ReadClassLabels(strLabelFilePath);

	
	BeginWaitCursor();

	// 图像预处理
	COCRImageProcess OCRImgBk = m_OCRImg; //原始图像备份
	m_OutOCRImg = m_OCRImg;

	
	m_OCRImg.Threshold(&m_OutOCRImg, nThres); //二值化处理
	m_OCRImg = m_OutOCRImg;

	m_OCRImg.DelScatterNoise(&m_OutOCRImg, nLower, nUpper);//去除离散噪声点
	m_OCRImg = m_OutOCRImg;		

	m_OCRImg.SlopeAdjust(&m_OutOCRImg); //倾斜度调整
	m_OCRImg = m_OutOCRImg;
	
	m_vecRT = m_OCRImg.ObjectSegment(); //字符分割
		
	m_OutOCRImg.ImResize(2*m_OCRImg.GetHeight(), 2*m_OCRImg.GetWidthPixel());//扩大目标图像，防止归一化后超出范围
	m_OCRImg.ObjectNorm(&m_OutOCRImg, IMG_WIDTH, IMG_HEIGHT, m_vecRT); //归一化为统一大小
	m_OCRImg = m_OutOCRImg;

	m_OCRImg.ObjectAlign(&m_OutOCRImg, m_vecRT); //字符对齐
	
	
	EndWaitCursor();
	m_OCRImg = OCRImgBk; //复原原始图像

	m_bOut = 1; //切换到显示输出图像的模式
	Invalidate();

	if(bTrain)//是否是训练模式
		SaveToTrainDIR(strDIR); //保存至训练分类目录
	else
		SaveToTestDIR(strDIR); //保存至测试目录
}



void CDigitRecView::OnFileOpen() 
{
	// 打开文件对话框
	char szFilter[] =
		"BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
 
	CFileDialog openfile(TRUE, NULL, NULL,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if(openfile.DoModal() != IDOK)
		return;


	if(!m_OCRImg.AttachFromFile(openfile.GetPathName()))
	{
		MessageBox(" 加载失败！");
		return;
	}

	
	// 在标题栏显示文件路径
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SetWindowText(openfile.GetPathName());

	m_bOut = 0;

	CSize sizeTotal;

	sizeTotal.cx = m_OCRImg.GetWidthPixel();
	sizeTotal.cy = m_OCRImg.GetHeight();
	SetScrollSizes(MM_TEXT, sizeTotal);



	InvalidateRect(NULL);		
}





void CDigitRecView::OnUpdatePreprocess(CCmdUI* pCmdUI) 
{
	if(!m_OCRImg.IsValidate())	
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}



void CDigitRecView::OnFileSaveAs() 
{
	if(!m_OCRImg.IsValidate())
		return;
	
	CFileDialog savefile(FALSE);
	if(!savefile.DoModal())
		return;
	
	
	if(m_bOut)
	{
		if(!m_OutOCRImg.SaveToFile(savefile.GetPathName()))
			MessageBox(" 文件保存失败");
	}
	else
	{
		if(!m_OCRImg.SaveToFile(savefile.GetPathName()))
			MessageBox(" 文件保存失败");
	}		
}



void CDigitRecView::OnFileRotate() 
{
	// 交换前后台图像
	m_bOut = !m_bOut;
	Invalidate();

}
