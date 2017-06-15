



#include "stdafx.h"
#include "neuralnet.h"
#include "../mainfrm.h"

// 在等待期间处理消息
void WaitForIdle() 
{
	MSG msg;  
	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}


CNeuralNet::CNeuralNet(int nInput, int nOutput, int nNeuronsPerLyr)
{
	m_nHiddenLayer = 1; //暂时只支持一个隐藏层的网络
	m_nInput = nInput;
	m_nOutput = nOutput;
	m_nNeuronsPerLyr = nNeuronsPerLyr;
		
	m_pHiddenLyr = NULL;
	m_pOutLyr = NULL;

	CreateNetwork(); //为网络各层分配空间
	InitializeNetwork(); // 初始化整个网络
}


CNeuralNet::~CNeuralNet()
{
	if(m_pHiddenLyr != NULL)
		delete m_pHiddenLyr;
	if(m_pOutLyr != NULL)
		delete m_pOutLyr;
}


void CNeuralNet::CreateNetwork()
{	
	m_pHiddenLyr = new SNeuronLayer(m_nNeuronsPerLyr, m_nInput);
	m_pOutLyr = new SNeuronLayer(m_nOutput, m_nNeuronsPerLyr);

}



void CNeuralNet::InitializeNetwork()
{
	int i, j; //循环变量


	//使用当前时间作为随机种子，这样可以保证程序每次运行产生不同的伪随机序列
	srand((unsigned)time(NULL));
	
	
	//初始化隐藏层权值
	for(i=0; i<m_pHiddenLyr->m_nNeuron; i++)
	{
		for(j=0; j<m_pHiddenLyr->m_pNeurons[i].m_nInput; j++)
		{
			m_pHiddenLyr->m_pNeurons[i].m_pWeights[j] = RandomClamped();
		
#ifdef NEED_MOMENTUM
			//第1个时代的训练开始之前，还没有上一次的权值更新信息
			m_pHiddenLyr->m_pNeurons[i].m_pPrevUpdate[j] = 0;
#endif
		}
	}

	//初始化输出层权值
	for(i=0; i<m_pOutLyr->m_nNeuron; i++)
	{
		for(j=0; j<m_pOutLyr->m_pNeurons[i].m_nInput; j++)
		{
			m_pOutLyr->m_pNeurons[i].m_pWeights[j] = RandomClamped();

#ifdef NEED_MOMENTUM
			//第1个时代的训练开始之前，还没有上一次的权值更新信息
			m_pOutLyr->m_pNeurons[i].m_pPrevUpdate[j] = 0;
#endif
		}
	}

	m_dErrorSum = 9999.0;//初识化为一个很大的训练误差，将随着训练进行逐渐减小
	m_nEpochs = 0; //当前训练时代数目
}

bool CNeuralNet::SaveToFile(const char* lpszFileName, bool bCreate)
{
	CFile file;
	if( bCreate ) //新建模式
	{
		if(!file.Open(lpszFileName, CFile::modeWrite|CFile::modeCreate))
			return false;
	}
	else//追加模式
	{
		if(!file.Open(lpszFileName, CFile::modeWrite))
			return false;

		file.SeekToEnd(); //追加写入到末尾
	}

	//写入网络头信息
	NEURALNET_HEADER header = { 0 };
	header.dwVersion = NEURALNET_VERSION;
	header.m_nInput = m_nInput;
	header.m_nNeuronsPerLyr = m_nNeuronsPerLyr;
	header.m_nOutput = m_nOutput;
	header.m_nEpochs = m_nEpochs; 
	file.Write(&header, sizeof(header));

	//写入训练误差信息
	file.Write(&m_dErr, sizeof(m_dErr));

	int i, j;
	//写入权值信息
	for(i=0; i<m_pHiddenLyr->m_nNeuron; i++)
	{//隐藏层权值
		file.Write(&m_pHiddenLyr->m_pNeurons[i].m_dActivation,
			sizeof(m_pHiddenLyr->m_pNeurons[i].m_dActivation));
		file.Write(&m_pHiddenLyr->m_pNeurons[i].m_dError, 
			sizeof(m_pHiddenLyr->m_pNeurons[i].m_dError));

		for(j=0; j<m_pHiddenLyr->m_pNeurons[i].m_nInput; j++)
		{
			file.Write(&m_pHiddenLyr->m_pNeurons[i].m_pWeights[j],
				sizeof(m_pHiddenLyr->m_pNeurons[i].m_pWeights[j]));
		}
	}

	for(i=0; i<m_pOutLyr->m_nNeuron; i++)
	{//输出层权值
		file.Write(&m_pOutLyr->m_pNeurons[i].m_dActivation,
			sizeof(m_pOutLyr->m_pNeurons[i].m_dActivation));
		file.Write(&m_pOutLyr->m_pNeurons[i].m_dError, 
			sizeof(m_pOutLyr->m_pNeurons[i].m_dError));
		for(j=0; j<m_pOutLyr->m_pNeurons[i].m_nInput; j++)
		{
			file.Write(&m_pOutLyr->m_pNeurons[i].m_pWeights[j],
				sizeof(m_pOutLyr->m_pNeurons[i].m_pWeights[j]));
		}
	}
	
	file.Close();

	return true;
}

bool CNeuralNet::LoadFromFile(const char* lpszFileName, DWORD dwStartPos)
{
	CFile file;
	if(!file.Open(lpszFileName, CFile::modeRead))
		return false;

	file.Seek(dwStartPos, CFile::begin);//定位到dwStartPos指出的开始位置

	//读入网络头信息
	NEURALNET_HEADER header = { 0 };
	if(file.Read(&header, sizeof(header)) != sizeof(header))
		return false;
	//校验版本
	if(header.dwVersion != NEURALNET_VERSION)
		return false;
	//校验网络基本配置
	if(m_nInput != header.m_nInput
			|| m_nNeuronsPerLyr != header.m_nNeuronsPerLyr
			|| m_nOutput != header.m_nOutput)
			return false;

	m_nEpochs = header.m_nEpochs; //更新训练时代数目
	file.Read(&m_dErr , sizeof(m_dErr));//读入训练误差信息

	int i, j;
	// 读入网络权值
	for(i=0; i<m_pHiddenLyr->m_nNeuron; i++)
	{//隐藏层
		file.Read(&m_pHiddenLyr->m_pNeurons[i].m_dActivation,
			sizeof(m_pHiddenLyr->m_pNeurons[i].m_dActivation));
		file.Read(&m_pHiddenLyr->m_pNeurons[i].m_dError, 
			sizeof(m_pHiddenLyr->m_pNeurons[i].m_dError));

		for(j=0; j<m_pHiddenLyr->m_pNeurons[i].m_nInput; j++)
		{
			file.Read(&m_pHiddenLyr->m_pNeurons[i].m_pWeights[j],
				sizeof(m_pHiddenLyr->m_pNeurons[i].m_pWeights[j]));
		}
	}

	for(i=0; i<m_pOutLyr->m_nNeuron; i++)
	{//输出层
		file.Read(&m_pOutLyr->m_pNeurons[i].m_dActivation,
			sizeof(m_pOutLyr->m_pNeurons[i].m_dActivation));
		file.Read(&m_pOutLyr->m_pNeurons[i].m_dError, 
			sizeof(m_pOutLyr->m_pNeurons[i].m_dError));
		for(j=0; j<m_pOutLyr->m_pNeurons[i].m_nInput; j++)
		{
			file.Read(&m_pOutLyr->m_pNeurons[i].m_pWeights[j],
				sizeof(m_pOutLyr->m_pNeurons[i].m_pWeights[j]));
		}
	}

	file.Close();
	return true;
}



bool CNeuralNet::CalculateOutput(vector<double> input, vector<double> &output)
{
	
	if(input.size() != m_nInput)//输入特征向量维数与网络输入不相等
		return false;

	int i, j;
	double nInputSum; //求和项

	// 计算隐藏层输出
	for(i=0; i<m_pHiddenLyr->m_nNeuron; i++)
	{
		nInputSum = 0;
		// 点乘计算
		for(j=0; j<m_pHiddenLyr->m_pNeurons[i].m_nInput-1; j++)
		{
			nInputSum += m_pHiddenLyr->m_pNeurons[i].m_pWeights[j]*(input[j]);
		}
		
		// 加上偏移项
		nInputSum += m_pHiddenLyr->m_pNeurons[i].m_pWeights[j]*BIAS;

		// 计算S函数的输出
		m_pHiddenLyr->m_pNeurons[i].m_dActivation = Sigmoid(nInputSum);
	}

	// 计算输出层输出
	for(i=0; i<m_pOutLyr->m_nNeuron; i++)
	{
		nInputSum = 0;
		// 点乘计算
		for(j=0; j<m_pOutLyr->m_pNeurons[i].m_nInput-1; j++)
		{
			nInputSum += m_pOutLyr->m_pNeurons[i].m_pWeights[j]
				*m_pHiddenLyr->m_pNeurons[j].m_dActivation;
		}

		// 加上偏移项
		nInputSum += m_pOutLyr->m_pNeurons[i].m_pWeights[j]*BIAS;

		// 计算S函数的输出
		m_pOutLyr->m_pNeurons[i].m_dActivation = Sigmoid(nInputSum);

		// 存入输出向量
		output.push_back(m_pOutLyr->m_pNeurons[i].m_dActivation);
	}

	return true;
}

bool CNeuralNet::TrainingEpoch(vector<iovector>& SetIn, vector<iovector>& SetOut)
{
	int i, j, k;

	double WeightUpdate; //权值更新量
	double err; //误差项


	m_dErrorSum = 0; //累计误差
	for(i=0; i<SetIn.size(); i++) //增量的梯度下降（针对每个训练样本更新权值）
	{
		//////////////////////////////
		iovector vecOutputs;

		if(!CalculateOutput(SetIn[i], vecOutputs)) //将输入沿网络向前传播
		{
			return false;
		}


		// 更新输出层权重		
		for(j=0; j<m_pOutLyr->m_nNeuron; j++)
		{
			// 计算误差项
			err = ((double)SetOut[i][j] - vecOutputs[j])*vecOutputs[j]*(1-vecOutputs[j]);
			m_pOutLyr->m_pNeurons[j].m_dError = err; //记录该单元的误差项

			m_dErrorSum += ((double)SetOut[i][j] - vecOutputs[j])*((double)SetOut[i][j] - vecOutputs[j]); //更新累计误差

			// 更新每个输入的权重
			for(k=0; k<m_pHiddenLyr->m_nNeuron; k++)
			{
				WeightUpdate = 
					err*m_dLearningRate*m_pHiddenLyr->m_pNeurons[k].m_dActivation;

#ifdef NEED_MOMENTUM
				// 带有冲量项的权值更新量
				m_pOutLyr->m_pNeurons[j].m_pWeights[k] +=
						WeightUpdate + m_pOutLyr->m_pNeurons[j].m_pPrevUpdate[k]*MOMENTUM;

				m_pOutLyr->m_pNeurons[j].m_pPrevUpdate[k] = WeightUpdate;
#else


				//更新单元权值
				m_pOutLyr->m_pNeurons[j].m_pWeights[k] += WeightUpdate;

#endif
			}

			//偏置更新量
			WeightUpdate = err*m_dLearningRate*BIAS;

#ifdef NEED_MOMENTUM
				// 带有冲量项的权值更新量
			m_pOutLyr->m_pNeurons[j].m_pWeights[k] +=
					WeightUpdate + m_pOutLyr->m_pNeurons[j].m_pPrevUpdate[k]*MOMENTUM;

			m_pOutLyr->m_pNeurons[j].m_pPrevUpdate[k] = WeightUpdate;
#else

			//偏置项的更新
			m_pOutLyr->m_pNeurons[j].m_pWeights[k] += WeightUpdate;

#endif
		}



		// 更新隐藏层权重
		for(j=0; j<m_pHiddenLyr->m_nNeuron; j++)
		{
			err = 0;
			
			for(k=0; k<m_pOutLyr->m_nNeuron; k++)
			{
				err += m_pOutLyr->m_pNeurons[k].m_dError*m_pOutLyr->m_pNeurons[k].m_pWeights[j];
			}

			err *= m_pHiddenLyr->m_pNeurons[j].m_dActivation*(1-m_pHiddenLyr->m_pNeurons[j].m_dActivation);
			
			
			// 更新每个输入的权重
			for(k=0; k<m_pHiddenLyr->m_pNeurons[j].m_nInput-1; k++)
			{
				WeightUpdate = err*m_dLearningRate*SetIn[i][k];

#ifdef NEED_MOMENTUM
				// 带有冲量项的权值更新量
				m_pHiddenLyr->m_pNeurons[j].m_pWeights[k] +=
						WeightUpdate + m_pHiddenLyr->m_pNeurons[j].m_pPrevUpdate[k]*MOMENTUM;

				m_pHiddenLyr->m_pNeurons[j].m_pPrevUpdate[k] = WeightUpdate;
#else



				m_pHiddenLyr->m_pNeurons[j].m_pWeights[k] += WeightUpdate;

#endif
			
			}
			//偏置更新量
			WeightUpdate = err*m_dLearningRate*BIAS;

#ifdef NEED_MOMENTUM
				// 带有冲量项的权值更新量
				m_pHiddenLyr->m_pNeurons[j].m_pWeights[k] +=
						WeightUpdate + m_pHiddenLyr->m_pNeurons[j].m_pPrevUpdate[k]*MOMENTUM;

				m_pHiddenLyr->m_pNeurons[j].m_pPrevUpdate[k] = WeightUpdate;
#else
			
			//偏置项的更新
			m_pHiddenLyr->m_pNeurons[j].m_pWeights[k] += WeightUpdate;

#endif
		}
	}

	m_nEpochs ++; //时代计数+1

	return true;
}



bool CNeuralNet::Train(vector<iovector>& SetIn, vector<iovector>& SetOut)
{
	m_bStop = FALSE; //是否要中途停止训练

	CString strOutMsg; //输出信息

	do
	{	
		//训练一个时代
		if(!TrainingEpoch(SetIn, SetOut)) 
		{
			
			strOutMsg.Format("训练在第%d个时代出现错误！", GetEpoch());
			AfxMessageBox(strOutMsg);
			return false;
		}

		
		//计算一个时代的平均到每一次训练、每个输出的错误
		m_dErr = GetErrorSum() / ( GetNumOutput() * SetIn.size() );

		if(m_dErr < m_dMinError)
			break; //收敛
		m_vecError.push_back(m_dErr); //记录各个时代的错误，以备训练结束后绘制训练误差曲线
	
		WaitForIdle(); //在循环中暂停下来以检查是否有用户动作和消息，主要为了让训练可以在中途停止
		
		if(m_bStop) //检查停止标志
			break;
	}
	while(m_nMaxEpoch-- > 0);
		

	return true;
}










/***********************
功能：
	识别某一个未知类别样本，返回类别标号
输入：
	CString strPathName: 待识别样本的存取路径
	rt:与图像等大的矩形区域
	dConfidence: 最大输出值与次大输出值的差，可以看作是识别的置信度
返回值：
	识别出的样本标号,返回-1表示识别失败
	
***********************/

int CNeuralNet::Recognize(CString strPathName, CRect rt, double &dConfidence)
{
	int nBestMatch; //类别标号
	
	double dMaxOut1 = 0; //最大输出
	double dMaxOut2 = 0; //次大输出

	//读入待识别图像文件
	CImg gray;
	if(!gray.AttachFromFile(strPathName))
		return -1;
	
	//将待识别图像转换为向量形式
	vector<double> vecToRec;
	for(int j=rt.top; j<rt.bottom; j+= RESAMPLE_LEN)
	{
		for(int i=rt.left; i<rt.right; i+= RESAMPLE_LEN)
		{				
			int nGray = 0;
			for(int mm=j; mm<j+RESAMPLE_LEN; mm++)
			{
				for(int nn=i; nn<i+RESAMPLE_LEN; nn++)
					nGray += gray.GetGray(nn, mm);
			}
			nGray /= RESAMPLE_LEN*RESAMPLE_LEN;
		

			vecToRec.push_back(nGray/255.0);
		}
	}
	
	
	//计算网络输出
	vector<double> outputs; //输出向量	
	if(!CalculateOutput(vecToRec, outputs))
	{
		AfxMessageBox("rec failed");
		return -1;
	}
	

	//寻找最大响应的的输出单元,对应的单元号即位类别
	nBestMatch = 0;
	for (int i=0; i<outputs.size(); ++i)
	{
		if (outputs[i] > dMaxOut1)
		{	
			dMaxOut2 = dMaxOut1;
			dMaxOut1 = outputs[i]; //记录最大的输出单元值
			
			
			nBestMatch = i;			
		}
	}

	dConfidence = dMaxOut1 - dMaxOut2; //计算置信度

	return nBestMatch; //返回识别结果（类标号）
}













