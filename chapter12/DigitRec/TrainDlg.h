#if !defined(AFX_TRAINDLG_H__D80133BB_A5BE_47D3_8622_A277B6BB287D__INCLUDED_)
#define AFX_TRAINDLG_H__D80133BB_A5BE_47D3_8622_A277B6BB287D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "neuralnet\\ValueTrack.h"




// TrainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrainDlg dialog

class CTrainDlg : public CDialog
{
// Construction
public:
	UINT m_nTimer; //定时器ID
	
	CRect m_rt; //图像处理的矩形区域
	CNeuralNet *m_pNet; //神经网络对象
	CNeuralData m_data; //神经网络的数据对象

	BOOL m_bStop;//停止训练标志

	CString strDirTrain; //训练样本分类目录
	CValueTrack *m_pTrack; //用于误差跟踪的对象
	

	BOOL m_bInTraining; //是否处于训练中

	CTrainDlg(CWnd* pParent = NULL);   // standard constructor
	~CTrainDlg();
	
	void SetDir(CString strDir); //设定训练分类目录
	void UpdateTrainInfo(); //更新训练误差曲线和时代、误差信息
	void EnableControls(BOOL bEnable); //设定控件状态
	bool SaveToFile(CString strFilePath); //保存训练配置文件
	bool LoadFromFile(CString strFilePathName, bool bLoadAll = true); //载入训练配置文件
// Dialog Data
	//{{AFX_DATA(CTrainDlg)
	enum { IDD = IDD_DIALOG_TRAIN };
	CEdit	m_EditFile;
	CEdit	m_EditTrainFile;
	CStatic	m_StaticShow;
	CStatic	m_StaticErr;
	int		m_nMaxEpoch;
	double	m_dError;
	double	m_dLearningRate;
	int		m_nNeurons;
	UINT	m_nTimeStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonTrain();
	afx_msg void OnButtonStop();
	afx_msg void OnClose();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TrainDLG_H__D80133BB_A5BE_47D3_8622_A277B6BB287D__INCLUDED_)
