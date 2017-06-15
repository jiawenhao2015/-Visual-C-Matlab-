
////////////////////////////////////////////////////////
// neuron.h文件


#ifndef __NEURON_H__
#define __NEURON_H__

#include <vector>

using namespace std;

#define NEURALNET_VERSION 0x03






// #define NEED_MOMENTUM //如需要增加冲量项，请将这一项的注释去掉

#define MOMENTUM 0.6 //冲量系数，仅在增加冲量项(#define NEED_MOMENTUM)的时候起作用

typedef double WEIGHT_TYPE; //定义权值的数据类型




struct SNeuron  // 神经细胞，神经元
{
	/////////////////////////////////数据//////////////////////////////

	int m_nInput; //输入数目
	WEIGHT_TYPE *m_pWeights; //对应输入的权值数组

#ifdef NEED_MOMENTUM

	WEIGHT_TYPE *m_pPrevUpdate; //在引入冲量项时用于记录上一次的权值更新
#endif


	// 激励值 输出值   经过S函数之后的值
	double m_dActivation;

	// 误差值
	double m_dError;



	
	/////////////////////////////////方法//////////////////////////////

	void Init(int nInput)
	{
		m_nInput = nInput+1; //由于有一个偏置项，输入数目是实际输入数目+1
		m_pWeights = new WEIGHT_TYPE[m_nInput]; //为权值数组分配空间

#ifdef NEED_MOMENTUM
		m_pPrevUpdate = new WEIGHT_TYPE[m_nInput]; //为上一次权值数组分配空间
#endif
	
		m_dActivation = 0; //神经元响应(输出)，经过Sigmoid激励函数后
		m_dError = 0; //神经元的误差值
	}

	~SNeuron()
	{
		//释放空间
		delete []m_pWeights;
#ifdef NEED_MOMENTUM
		delete []m_pPrevUpdate;
#endif
	}	
	
}; //SNeuron



struct SNeuronLayer //神经网络层
{
	/////////////////////////////////数据//////////////////////////////

	int m_nNeuron; //该层的神经元数目
	SNeuron *m_pNeurons; //神经元数组



	////////////////////////////////方法////////////////////////////////

	SNeuronLayer(int nNeuron, int nInputsPerNeuron)
	{
		m_nNeuron = nNeuron;
		m_pNeurons = new SNeuron[nNeuron]; //分配nNeuron个神经元的数组空间

		for(int i=0; i<nNeuron; i++)
		{
			m_pNeurons[i].Init(nInputsPerNeuron); //神经元初始化
		}
	}
	~SNeuronLayer()
	{
		delete []m_pNeurons; //释放神经元数组
	}
}; //SNeuronLayer




//////////////////保存训练文件时使用/////////////
struct NEURALNET_HEADER
{
	DWORD dwVersion; //版本信息
	
	// 初始化参数，不可更改
	int m_nInput; //网络输入数目
	int m_nOutput; //网络输出单元数目
	int m_nHiddenLayer; //隐藏层数目，DigitRec中只支持1个隐藏层

	//可在每次训练前设置的参数
	int m_nNeuronsPerLyr;	//隐藏层单元数目	
	int m_nEpochs; //训练时代数目（反向传播算法的迭代次数）
}; //NEURALNET_HEADER

#endif // __NEURON_H__










