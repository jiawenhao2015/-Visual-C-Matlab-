// TrainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DigitRec.h"
#include "TrainDlg.h"
#include "DirDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrainDlg dialog


CTrainDlg::CTrainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrainDlg)
	m_nMaxEpoch = 8000;
	m_dError = 0.001;
	m_dLearningRate = 0.1;
	m_nNeurons = 4;
	m_nTimeStep = 100;
	//}}AFX_DATA_INIT

	m_pNet = NULL;
	m_pTrack = NULL;
	m_bInTraining = false;
	
	
}

CTrainDlg::~CTrainDlg()
{
	if( m_pNet != NULL )
		delete m_pNet;
	
	if( m_pTrack != NULL )
		delete m_pTrack;
	
}


void CTrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrainDlg)
	DDX_Control(pDX, IDC_EDIT_FILE, m_EditFile);
	DDX_Control(pDX, IDC_EDIT_TRAIN_FILE, m_EditTrainFile);
	DDX_Control(pDX, IDC_STATIC_SHOW, m_StaticShow);
	DDX_Control(pDX, IDC_STATIC_ERROR, m_StaticErr);
	DDX_Text(pDX, IDC_EDIT_MAXEPOCH, m_nMaxEpoch);
	DDX_Text(pDX, IDC_EDIT_ERROR, m_dError);
	DDX_Text(pDX, IDC_EDIT_LEARNRATE, m_dLearningRate);
	DDX_Text(pDX, IDC_EDIT_HIDLYR, m_nNeurons);
	DDX_Text(pDX, IDC_EDIT_TIMESTEP, m_nTimeStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrainDlg, CDialog)
	//{{AFX_MSG_MAP(CTrainDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_BUTTON_TRAIN, OnButtonTrain)
	ON_BN_CLICKED(ID_BUTTON_STOP, OnButtonStop)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, OnButtonSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainDlg message handlers

BOOL CTrainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pTrack = new CValueTrack(&m_StaticErr);
	
	
	CRect rt(0, 0, IMG_WIDTH, IMG_HEIGHT); //设置一个与图像大小相同的矩形
	m_rt = rt;


	// 计算输入个数
	int nInputs = 0;
	for(int jj=m_rt.top; jj<m_rt.bottom; jj+=RESAMPLE_LEN)
	{
		for(int ii=m_rt.left; ii<m_rt.right; ii+=RESAMPLE_LEN)
		{
			nInputs ++;
		}
	}
	m_data.Init(rt, nInputs);


	//设置按钮初始状态
	( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(true);
	( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrainDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_pTrack->Draw(); //绘制错误跟踪区域


	// Do not call CDialog::OnPaint() for painting messages
}

void CTrainDlg::UpdateTrainInfo()
{
	// 取得当前误差并绘制误差曲线
	double dErr = m_pNet->GetError();
	m_pTrack->AddValue(dErr);
	m_pTrack->Draw();

	// 更新时代和误差信息，显示在对话框底部
	int nEpoch = m_pNet->GetEpoch();
	CString strOut;
	strOut.Format("时代:%d, 误差:%f", nEpoch, dErr);
	m_StaticShow.SetWindowText(strOut);
}

void CTrainDlg::OnTimer(UINT nIDEvent) 
{
	
	if(m_bInTraining)
		UpdateTrainInfo(); // 定时跟踪绘制误差信息
	
	CDialog::OnTimer(nIDEvent);
}

void CTrainDlg::EnableControls(BOOL bEnable)
{
	( (CButton* )GetDlgItem(IDC_EDIT_TRAIN_FILE) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_BUTTON_SAVEAS) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_FILE) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_BUTTON_BROWSE) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_MAXEPOCH) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_ERROR) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_LEARNRATE) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_HIDLYR) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_TIMESTEP) )->EnableWindow(bEnable);
}

void CTrainDlg::OnButtonTrain() 
{
	//更新UI状态
	( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(false);
	( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(true);
	EnableControls(false);
	
	int nOuputs = m_data.GetClassNum(); //输出单元数目(采用1ofn编码=类别数)
	int nInputs = m_data.GetInputNum(); //输入单元数目

	//设置定时器，一旦训练开始定时跟踪绘制误差
	m_nTimer = SetTimer(1, m_nTimeStep, NULL);

	//更新配置参数
	UpdateData(true);

	//创建网络
	if( m_pNet != NULL )
		delete m_pNet;
	m_pNet = new CNeuralNet(nInputs, nOuputs, m_nNeurons);
	
	//根据用户输入配置网络参数
	m_pNet->SetMaxEpoch(m_nMaxEpoch);
	m_pNet->SetMinError(m_dError);
	m_pNet->SetLearningRate(m_dLearningRate);
				
	CString strOut; //输出信息字符串

	//检查训练样本目录
	CString strFilePath;
	m_EditFile.GetWindowText(strFilePath);
	if(strFilePath.GetLength()==0)
	{//没有指定训练样本目录，无法开始训练
		AfxMessageBox("没有指定训练目录信息，无法开始训练!");
		m_bInTraining = false;
		( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(true);
		( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(false);
		EnableControls(true);
		return;		
	}

	//载入训练配置文件
	CString strTrainFilePath;
	m_EditTrainFile.GetWindowText(strTrainFilePath);
	if(strTrainFilePath.GetLength() == 0)
	{//没有指定训练配置文件，无法开始训练
		AfxMessageBox("请指定一个用于保存训练结果的训练配置文件以便开始训练!");
		m_bInTraining = false;
		( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(true);
		( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(false);
		EnableControls(true);
		return;		
	}
	if(!LoadFromFile(strTrainFilePath))
	{//新的训练
		AfxMessageBox("没有找到匹配的训练配置文件或隐藏层单元数目发生变化!重新训练网络.");
		m_pTrack->Init();
	}
	else//继续上次训练
		m_pTrack->Draw();

	//更新配置参数
	UpdateData(true);
	
	//在对话框标题栏显示网络结构信息
	int nNeurons = m_pNet->GetNumNeuronsPerLyr(); //隐藏层单元数目
	strOut.Format("inputs: %d outputs: %d nerons: %d",
		nInputs, nOuputs, nNeurons);
	SetWindowText(strOut);
		
	//统计训练时间开始
	DWORD dwEsplise = ::GetTickCount();	
	m_bInTraining = true; //开始训练
	
	m_pNet->Train(m_data.GetInputSet(), m_data.GetOutputSet());//训练
	
	//保存训练成果文件
	CString strFilePathName;
	m_EditTrainFile.GetWindowText(strFilePathName);
	SaveToFile(strFilePathName);

	KillTimer(m_nTimer);//删除计时器

	//最后更新一次训练误差情况
	UpdateTrainInfo();

	//统计训练时间结束
	dwEsplise = ::GetTickCount() - dwEsplise;

	strOut.Format(" 训练结束，用时：%d 秒", dwEsplise/1000);
	AfxMessageBox(strOut);

	//更新UI状态
	( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(true);
	( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(false);
	EnableControls(true);
	
	m_bInTraining = false; //训练结束
}



void CTrainDlg::OnButtonStop() 
{
	m_pNet->SetStopFlag(true); //设置m_pNet的停止训练标志，从而中止训练过程
	
	m_bInTraining = false;

	( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(true);
	( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(false);

	KillTimer(m_nTimer);
}

void CTrainDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if( m_bInTraining )
	{
		AfxMessageBox( _T("正在训练中!请首先停止训练再关闭窗口.") );
		return;
	}

	CDialog::OnClose();
}

void CTrainDlg::SetDir(CString strDir)
{
	// 设定分类目录相关信息
	m_data.GetClassInfoFromDir(strDir);
	
	//载入整个训练集合
	if(!m_data.CreateTrainingSetFromData()) 
	{
		MessageBox("建立训练集时出现错误!");
		return;
	}
	
	// 在编辑框显示用户选定的训练分类目录信息
	m_EditFile.SetWindowText(strDir);
}

void CTrainDlg::OnButtonBrowse() 
{
	//选择训练样本分类目录
	CDirDialog dir;
	if(!dir.DoBrowse(*this))
	{
		return;
	}
		
	SetDir(dir.GetPath());
	
}

bool CTrainDlg::SaveToFile(CString strFilePath)
{
	//保存其他配置信息
	CFile file;
	if(!file.Open(strFilePath, CFile::modeWrite|CFile::modeCreate))
		return false;
	
	//保存神经网络结构信息
	int nInput = m_data.GetInputNum(); //输入单元数
	int nOutput = m_data.GetClassNum(); //输出层单元数
	int nNeurons = m_nNeurons; //隐层单元数
	file.Write(&nInput, sizeof(nInput));
	file.Write(&nOutput, sizeof(nOutput));
	file.Write(&nNeurons, sizeof(nNeurons));
	file.Write(&m_dError, sizeof(m_dError));
	file.Write(&m_dLearningRate, sizeof(m_dLearningRate));
	file.Write(&m_nMaxEpoch, sizeof(m_nMaxEpoch));

	//保存类别名称信息
	vector<CString> vecClassNames = m_data.GetVecClassNames();
	int i;
	int nLen;
	for(i = 0; i<vecClassNames.size(); i++)
	{
		nLen = vecClassNames[i].GetLength();
		file.Write( &nLen, sizeof(nLen) );
		file.Write( (LPCTSTR)vecClassNames[i], nLen );
	}

	//保存训练目录和配置文件信息
	CString strPath;
	m_EditFile.GetWindowText(strPath);
	nLen = strPath.GetLength();
	file.Write( &nLen, sizeof(nLen) );
	file.Write( (LPCTSTR)strPath, strPath.GetLength() );

	file.Close();

	
	//保存神经网络训练配置信息
	if( !m_pNet->SaveToFile(strFilePath, false) ) //第二个参数false表示是追加写入
		return false;
	

	return true;
}



bool CTrainDlg::LoadFromFile(CString strFilePathName, bool bLoadAll)
{
	//载入其它配置信息
	CFile file;
	if(!file.Open(strFilePathName, CFile::modeRead))
		return false;

	//载入网络结构信息
	int nInput, nOutput;
	file.Read(&nInput, sizeof(nInput));
	file.Read(&nOutput, sizeof(nOutput));
	file.Read(&m_nNeurons, sizeof(m_nNeurons));
	file.Read(&m_dError, sizeof(m_dError));
	file.Read(&m_dLearningRate, sizeof(m_dLearningRate));
	file.Read(&m_nMaxEpoch, sizeof(m_nMaxEpoch));

	

	int nLen;
	char* lpBuf;
	int i;
	for(i=0; i<nOutput; i++)
	{
		file.Read(&nLen, sizeof(nLen));
		lpBuf = new char[nLen+1];
		file.Read(lpBuf, nLen);
		lpBuf[nLen] = '\0';
		delete [] lpBuf;		
	}

	file.Read(&nLen, sizeof(nLen));
	lpBuf = new char[nLen+1];
	file.Read(lpBuf, nLen);
	lpBuf[nLen] = '\0';
	CString strTrainPathName;
	strTrainPathName = lpBuf;
	delete [] lpBuf;
	m_EditFile.SetWindowText(strTrainPathName);

	DWORD dwCurPos = file.GetPosition(); //当前文件指针位置
	file.Close();
	if(!bLoadAll)
		return true;
	
	//载入神经网络训练配置信息
	if( !m_pNet->LoadFromFile(strFilePathName, dwCurPos) )
		return false;

	
	m_EditTrainFile.SetWindowText(strFilePathName);
	return  true;
}







void CTrainDlg::OnButtonSaveAs()
{
	CFileDialog saveDlg(FALSE, "net", "", NULL, "Neuralnet Config Files(*.net)|*.net||", NULL);
	
	if(saveDlg.DoModal() == IDOK)
	{
		CString strFilePath = saveDlg.GetPathName();
		m_EditTrainFile.SetWindowText(strFilePath);	

		//第二个参数为false表示只载入网络基本信息
		if(LoadFromFile(strFilePath, false))			
			UpdateData(false);

		m_pTrack->Init();
	}
	
}