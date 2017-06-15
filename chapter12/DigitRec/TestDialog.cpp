// TestDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DigitRec.h"
#include "TestDialog.h"
#include "DirDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDialog dialog


CTestDialog::CTestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDialog)
	//}}AFX_DATA_INIT
	m_vecClassNames.clear();
	m_pNet = NULL;
	m_bInTest = false;
	m_bReadyToRec = false;
}

CTestDialog::~CTestDialog()
{
	if( m_pNet != NULL )
		delete m_pNet;
}

void CTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDialog)
	DDX_Control(pDX, IDC_EDIT_FILE, m_EditFile);
	DDX_Control(pDX, IDC_BUTTON_TEST_BROWSE, m_ButtonTestBrowse);
	DDX_Control(pDX, IDC_EDIT_LOAD_TRAIN_FILE, m_EditLoadTrainFile);
	DDX_Control(pDX, IDC_CHECK_DIR, m_CheckDir);
	//}}AFX_DATA_MAP
}


bool CTestDialog::LoadFromFile(CString strFilePathName)
{
	//载入其它配置信息
	CFile file;
	if(!file.Open(strFilePathName, CFile::modeRead))
		return false;

	//载入网络结构信息
	int nInput, nOutput, nNeurons;
	file.Read(&nInput, sizeof(nInput));
	file.Read(&nOutput, sizeof(nOutput));
	file.Read(&nNeurons, sizeof(nNeurons));
	double dError, dLearningRate;
	int nMaxEpoch;
	file.Read(&dError, sizeof(dError));
	file.Read(&dLearningRate, sizeof(dLearningRate));
	file.Read(&nMaxEpoch, sizeof(nMaxEpoch));

	int nLen;
	char* lpBuf;
	int i;
	for(i=0; i<nOutput; i++)
	{
		file.Read(&nLen, sizeof(nLen));
		lpBuf = new char[nLen+1];
		file.Read(lpBuf, nLen);
		lpBuf[nLen] = '\0';
		CString strClassName = lpBuf;
		delete [] lpBuf;
		m_vecClassNames.push_back(strClassName);
	}

	file.Read(&nLen, sizeof(nLen));
	lpBuf = new char[nLen+1];
	file.Read(lpBuf, nLen);
	lpBuf[nLen] = '\0';
	CString strTrainPathName;
	strTrainPathName = lpBuf;
	delete [] lpBuf;
	

	DWORD dwCurPos = file.GetPosition(); //当前文件指针位置
	file.Close();
	
	if(m_pNet != NULL)
		delete m_pNet;
	m_pNet = new CNeuralNet(nInput, nOutput, nNeurons);
	//载入神经网络训练配置信息
	if( !m_pNet->LoadFromFile(strFilePathName, dwCurPos) )
		return false;

	
	m_EditLoadTrainFile.SetWindowText(strFilePathName);
	return  true;
}


BEGIN_MESSAGE_MAP(CTestDialog, CDialog)
	//{{AFX_MSG_MAP(CTestDialog)
	ON_BN_CLICKED(IDC_BUTTON_REC, OnButtonRec)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_TEST_BROWSE, OnButtonTestBrowse)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDialog message handlers

void CTestDialog::OnButtonRec() 
{
	CString strTrainFile;
	m_EditFile.GetWindowText(strTrainFile);//取得测试文件路径信息
	if(strTrainFile.GetLength() == 0)
	{
		AfxMessageBox("还没有选择识别文件或测试目录!");
		return;
	}
	if(!m_bReadyToRec)//没有指定训练配置文件
	{
		AfxMessageBox("请首先指定一个训练配置文件以便开始识别!");
		return;
	}

	m_bInTest = true; //测试中标志
	( (CButton* )GetDlgItem(IDC_BUTTON_STOP) )->EnableWindow(true); //启用停止按钮

	BOOL bDir = m_CheckDir.GetCheck(); //是否选定目录测试选项

	if(!bDir) //测试一个文件
	{
		CString strFile;
		m_EditFile.GetWindowText(strFile);
		double dConfidence; //置信度
		int nClass = m_pNet->Recognize(strFile, m_rt, dConfidence); //识别
		CString str;
		CString strRec;
		if(nClass >= 0)
		{
			strRec = m_vecClassNames[nClass];
			str.Format(" 识别结果: %s  置信度: %%%d ", strRec, (int)(dConfidence*100));
		}
		else
			str = "识别失败!";
				
		MessageBox(str);	
	}
	else //测试整个目录
	{
		WIN32_FIND_DATA findData;
		HANDLE hFindFile;
		CString str;

		//打开对话框，让用户选择一个保存识别结果的文件
		CFileDialog saveDlg(FALSE, "txt", "", NULL, "txt Files(*.txt)|*.txt||", NULL);
		if(saveDlg.DoModal() != IDOK)
		{
			( (CButton* )GetDlgItem(IDC_BUTTON_STOP) )->EnableWindow(false); //禁用停止按钮
			m_bInTest = false;
			return;		
		}

		CString strRecFile = saveDlg.GetPathName(); //识别结果文件
		CFile file;
		if(!file.Open(strRecFile, CFile::modeCreate|CFile::modeWrite))
		{
			MessageBox(" create result file failed!");
			( (CButton* )GetDlgItem(IDC_BUTTON_STOP) )->EnableWindow(false); //禁用停止按钮
			m_bInTest = false;
			return;
		}
		
		CString strDir;
		m_EditFile.GetWindowText(strDir);; //取得测试目录路径信息
		if(strDir[strDir.GetLength() - 1] != '\\')
		{
			strDir += '\\';
		}
			
		
		CString szFileName = strDir;
		szFileName += "*.bmp"; //搜索目标为目录下的所有bmp文件
		
		str = " \r\n  识别结果为: \r\n \r\n";
		file.Write(str, str.GetLength());//写识别结果文件
			
		int nTotal = 0; //测试样本总数
		int nNotRec = 0; //识别失败的次数
		double dConfidence; //置信度
		CImg ImageTmp;
		
		hFindFile = ::FindFirstFile(szFileName, &findData);
		if(hFindFile != INVALID_HANDLE_VALUE)
		{
			do
			{
				// 名称为“.”的目录代表本目录，名称为“..”的目录代表上一层目录
				if(findData.cFileName[0] == '.')
					continue;
				
				if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{// 找到的不是目录而是文件
					str = strDir;
					str += findData.cFileName;
					
					int nClass = m_pNet->Recognize(str, m_rt, dConfidence);

					CString strRec = "识别失败!";
					if(nClass >= 0)
						strRec = m_vecClassNames[nClass];
					else
						nNotRec ++;		

					CString strToWrite;
					strToWrite.Format(" 文件: %s  类别标号: %s  置信度: %%%d \r\n", 
						findData.cFileName, strRec, (int)(dConfidence*100));
					file.Write(strToWrite, strToWrite.GetLength()); //写入识别结果文件

					nTotal ++; //样本总数+1
				}
				
			}while(::FindNextFile(hFindFile, &findData));
			::FindClose(hFindFile);
		}
		
		str.Format("\r\n 识别结束, 总数: %d, 失败: %d \r\n", nTotal, nNotRec);
		file.Write(str, str.GetLength());
		file.Close();
		
		MessageBox(str);//显示测试信息

		ShellExecute(m_hWnd, "open", "notepad.exe", strRecFile, NULL, SW_SHOW); //打开识别结果文件
	}
	
	m_bInTest = false; //测试结束
}

BOOL CTestDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rt(0, 0, IMG_WIDTH, IMG_HEIGHT); //设置一个与图像大小相同的矩形
	m_rt = rt;
	
	( (CButton* )GetDlgItem(IDC_BUTTON_STOP) )->EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTestDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if( m_bInTest )
	{
		AfxMessageBox( _T("正在测试(识别)中!请首先停止测试再关闭窗口.") );
		return;
	}

	CDialog::OnClose();
}

void CTestDialog::OnButtonLoad() 
{
	// 让用户选择测试(识别)所用的训练配置文件(.net文件)

	CString strFilePathName;
	CFileDialog dlg(true, "net", "", NULL, "Neuralnet Config Files(*.net)|*.net||", NULL); //TRUE为OPEN对话框，FALSE为SAVE AS对话框
	
	if( dlg.DoModal() == IDOK )
		strFilePathName = dlg.GetPathName();

	//载入训练配置文件
	if( LoadFromFile(strFilePathName) )
		m_bReadyToRec = true;

}

void CTestDialog::OnButtonTestBrowse() 
{
	int bDir = m_CheckDir.GetCheck();
	if(!bDir)
	{
		CFileDialog openfile(TRUE, "bmp", "", NULL, "Bitmap Files(*.bmp)|*.bmp||", NULL);
		if(openfile.DoModal() != IDOK)
			return;

		if(!m_Img.AttachFromFile(openfile.GetPathName()))
		{
			return;
		}

		m_EditFile.SetWindowText(openfile.GetPathName());

		CRect rect;	
		GetDlgItem(IDC_STATICIMG)->GetWindowRect(rect);
			
		ScreenToClient(&rect);
			
		InvalidateRect(&rect);
	}
	else
	{
		//选择训练样本分类目录
		CDirDialog dir;
		if(!dir.DoBrowse(*this))
		{
			return;
		}
		m_EditFile.SetWindowText(dir.GetPath());
		
	}
}

CPen Text_Pen(PS_SOLID, 1, RGB(255, 0, 0));

void CTestDialog::OnPaint() 
{
	CPaintDC dc(this);	
	
	if(m_Img.IsValidate())
	{
		CRect rect;
		
		GetDlgItem(IDC_STATICIMG)->GetWindowRect(rect);
		
		ScreenToClient(&rect);
		
		m_Img.Draw(&dc, rect);

		HGDIOBJ pOld = dc.SelectObject(Text_Pen);

		dc.MoveTo(rect.left + m_rt.left, rect.top + m_rt.top);
		dc.LineTo(rect.left + m_rt.right, rect.top + m_rt.top);
		dc.LineTo(rect.left + m_rt.right, rect.top + m_rt.bottom);
		dc.LineTo(rect.left + m_rt.left, rect.top + m_rt.bottom);
		dc.LineTo(rect.left + m_rt.left, rect.top + m_rt.top);


		dc.SelectObject(pOld);
	}


	// Do not call CDialog::OnPaint() for painting messages
}

void CTestDialog::OnButtonStop() 
{
	m_bInTest = false;

	( (CButton* )GetDlgItem(IDC_BUTTON_REC) )->EnableWindow(true);
	( (CButton* )GetDlgItem(IDC_BUTTON_STOP) )->EnableWindow(false);	
}
