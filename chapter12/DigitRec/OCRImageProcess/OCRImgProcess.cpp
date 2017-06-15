// OCRImageProcess.cpp: implementation of the COCRImageProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../DigitRec.h"
#include "OCRImgProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COCRImageProcess::COCRImageProcess()
{

}

COCRImageProcess::~COCRImageProcess()
{

}

// 从基类的转换
COCRImageProcess& COCRImageProcess::operator = (CImg& img)
{
	// 拷贝图像大小
	if ( IsValidate() )
	{
		ImResize(img.GetHeight(), img.GetWidthPixel());
	}
	else
	{
		throw "Error in operaor '=': Left hand operand not initialized.";
		return *this;
	}

	// 拷贝图像数据
	for (int i=0; i<GetHeight(); i++)
	{
		memcpy(m_lpData[i], img.m_lpData[i], img.GetWidthByte() * sizeof(BYTE));
	}

	return *this;
}



/******************* 
void COCRImageProcess::SlopeAdjust(COCRImageProcess* pTo)
功能：
	调整图像中前景物体的倾斜度，使其尽量处于一个水平位置上。比较适合于横向分布的物体，如字符的调整
 注：
	只能处理2值图象

 参数：
 COCRImageProcess* pTo: 目标图像的 COCRImageProcess 指针
 
 返回值：
	无
 *******************/

void COCRImageProcess::SlopeAdjust(COCRImageProcess* pTo)
{
	int i,j;
	double dAvgLHeight = 0; //图像左半部分前景物体的平均高度
	double dAvgRHeight = 0; //图像右半部分前景物体的平均高度

	//取得图像的高和宽
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int nGray;
	int nWeightSum = 0; //统计平均加权高度时的权重和

	//逐行扫描左半部分图像，计算黑色像素的平均加权高度
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth/2; j++)
		{
			nGray = GetGray(j, i);
			if(nGray == 0)//是前景物体（黑）
			{
				//计算高度的加权和，给靠两边的像素分配更多的权重
				nWeightSum += nWidth/2 - j;
				dAvgLHeight += i*(nWidth/2 - j);
			}
		}//for j
	}//for i

	dAvgLHeight /= nWeightSum; //平均加权高度
	nWeightSum = 0;

	//逐行扫描右半部分图像，计算黑色像素的平均加权高度
	for(i=0; i<nHeight; i++)
	{
		for(j=nWidth/2; j<nWidth; j++)
		{
			nGray = GetGray(j, i);

			if(nGray == 0)//是前景物体（黑）
			{
				//计算高度的加权和，给靠两边的像素分配更多的权重
				nWeightSum += j - nWidth/2;
				dAvgRHeight += i*(j - nWidth/2);			
			}
		}//for j
	}//for i

	dAvgRHeight /= nWeightSum;

	//计算斜率
	double dSlope = (dAvgLHeight - dAvgRHeight) / (nWidth/2);
	int nYSrc; //y的源坐标

	//扫描新图像，根据斜率得到的新旧图像的映射关系为每一个像素赋值
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			//找到与新图像的当前点对应的旧图像点的水平坐标(以水平方向中点为中心)
			nYSrc = int(i - (j - nWidth/2) * dSlope);
			if( nYSrc < 0 || nYSrc >= nHeight ) //对应点在不在图像区域之内
				nGray = 255;
			else
				nGray = GetGray(j, nYSrc);
			pTo->SetPixel(j, i, RGB(nGray, nGray, nGray)); //根据源图像对应点像素值为新图像像素赋值
		}//for j
	}//for i
}



/******************* 
vector<RECT> COCRImageProcess::ObjectSegment()
功能：
	对前景目标（如字符）进行划分，将各个字符轮廓矩形返回
 注：
	只能处理2值图象

 参数：
	无
 
 返回值：
	vecRECT: 包含各个字符轮廓矩形的RECT向量
 *******************/
vector<RECT> COCRImageProcess::ObjectSegment()
{	
	vector<RECT> vecRoughRECT; //粗略对象轮廓的矩形向量数组
	vector<RECT> vecRECT; //精化后对象轮廓的矩形向量数组

	//清空用来表示每个对象区域的 vector
	vecRoughRECT.clear();
	vecRECT.clear();

	int i, j;
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	
	int nTop, nBottom; // 整体前景区域的上下边界
	int nGray; //像素灰度
	int nObjCnt = 0; //对象数目

	//从上向下逐行扫描，找到整体前景区域的上边界
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			nGray = GetGray(j, i);
			if(nGray == 0)
			{
				nTop = i;
				i = nHeight; //对i赋大值，使得在break跳出内层循环后，直接可以再跳出外层循环
				break;
			}
		}// for j
	}// for i

	//从下向上逐行扫描，找到整体前景区域的下边界
	for(i=nHeight-1; i>=0; i--)
	{
		for(j=0; j<nWidth; j++)
		{
			nGray = GetGray(j, i);
			if(nGray == 0)
			{
				nBottom = i;
				i = -1; //对i赋小值，使得在break跳出内层循环后，直接可以再跳出外层循环
				break;
			}
		}// for j
	}// for i

	bool bStartSeg = false; //是否已开始某一个对象的分割
	bool bBlackInCol; //某一列中是否包含黑色像素

	RECT rt;

	//按列扫描，找到每一个目标的左、右边界
	for(j=0; j<nWidth; j++)
	{
		bBlackInCol = false;
		for(i=0; i<nHeight; i++)
		{
			nGray = GetGray(j, i);
			if( nGray == 0 )
			{
				bBlackInCol = true; //该列中发现黑点
				if(!bStartSeg) //还没有进入一个对象的分割
				{
					rt.left = j;

					bStartSeg = true; //目标分割开始
				}
				else //仍处于某一个对象之内
					break;
			}// if( gray == 0 )
		}// for i

		if( j == (nWidth-1) ) //扫描到最右一列了,说明整个图像扫描完毕
			break;
		if(bStartSeg && !bBlackInCol) //正处在分割状态且扫描完一列都没有发现黑像素，表明当前对象分割结束
		{
			rt.right = j; //对象右边界确定

			//对象的粗略上下边界（有待精化）
			rt.top = nTop;
			rt.bottom = nBottom;

			::InflateRect(&rt, 1, 1); //矩形框膨胀1个像素，以免绘制时压到字符

			vecRoughRECT.push_back(rt); //插入 vector

			bStartSeg = false; //当前分割结束
			nObjCnt ++; //对象数目加1
		}

		//进入下一列的扫描
	}// for j
	
	RECT rtNew; //存放精化对象区域的矩形框

	//由于已经得到了精确的左、右边界，现在可以精化矩形框的上下边界
	int nSize = vecRoughRECT.size();
	for(int nObj=0; nObj<nSize; nObj++)
	{

		rt = vecRoughRECT[nObj];

		rtNew.left = rt.left - 1;
		rtNew.right = rt.right + 1;

		//从上向下逐行扫描确定上边界
		for(i=rt.top; i<rt.bottom; i++)
		{
			for(j=rt.left; j<rt.right; j++)
			{
				nGray = GetGray(j, i);
				if(nGray == 0)
				{
					rtNew.top = i-1;
					i = rt.bottom; //对i赋大值，使得在break跳出内层循环后，直接可以再跳出外层循环
					break;
				}
			}// for j
		}//for i


		//从下向上逐行扫描确定下边界
		for(i=rt.bottom-1; i>=rt.top; i--)
		{
			for(j=rt.left; j<rt.right; j++)
			{
				nGray = GetGray(j, i);
				if(nGray == 0)
				{
					rtNew.bottom = i+1;
					i = rt.top-1; //对i赋小值，使得在break跳出内层循环后，直接可以再跳出外层循环
					break;
				}
			}// for j
		}//for i

		vecRECT.push_back(rtNew);
	}//for nObj

	return vecRECT; //返回包含了各个分割后字符的轮廓矩形
}




/******************* 
void COCRImageProcess::ObjectNorm(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, vector<RECT> &vecRT)
功能：
	对各个对象进行尺寸的归一化处理，以他们具有相同的宽和高，以方便特征的提取。
	应在提取了对象轮廓矩形之后使用
注：
	只能处理2值图象
	需要矩形轮廓向量作为输入参数，应在提取了对象轮廓矩形之后使用

参数：
	COCRImageProcess* pTo: 目标图像的 COCRImageProcess 指针
	nTargWidth: 归一化的目标宽度
	nTargHeight: 归一化的目标高度
	vecRT: 表示字符分割的矩形向量数组
 
返回值：
	无
*******************/
void COCRImageProcess::ObjectNorm(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, vector<RECT> &vecRT)
{
	pTo->InitPixels(255); //目标图像置白

	int nSize = vecRT.size(); //取得对象(字符)矩形轮廓数目


	//缩放每个轮廓矩形到标准尺寸
	for(int nObj=0; nObj<nSize; nObj++)
	{
		RECT rt = vecRT[nObj]; //取得一个轮廓矩形
		
		vecRT[nObj] = RgnZoom(pTo, nTargWidth, nTargHeight, &rt);
	}
	
}

 



/******************* 
void COCRImageProcess::RgnZoom(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, LPRECT lpRect)
功能：
	将 pTo 指向的图像中由 lpRect 指示的矩形区域缩放置 nTargWidth 宽，nTargHeight 高
注：
	只能处理2值图象，nTargWidth 和 nTargHeight 给出的大小不能超出图像范围

参数：
	COCRImageProcess* pTo: 目标图像的 COCRImageProcess 指针
	nTargWidth: 归一化的目标宽度
	nTargHeight: 归一化的目标高度
	lpRect: 标准化缩放之前的轮廓矩形
 
返回值：
	缩放后的区域矩形
********************/
RECT COCRImageProcess::RgnZoom(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, LPRECT lpRect)
{
	RECT retRT; //缩放后的区域矩形

	double dXScale; //水平方向的缩放因子
	double dYScale; //竖直方向的缩放因子

	int i, j;
	
	
	//确定缩放系数
	dXScale = (double)nTargWidth / (lpRect->right - lpRect->left + 1);
	dYScale = (double)nTargHeight / (lpRect->bottom - lpRect->top + 1);

	int nSrc_i, nSrc_j; //映射源坐标


	retRT.top = lpRect->top;
	retRT.bottom = retRT.top + nTargHeight;
	retRT.left = lpRect->left;
	retRT.right = retRT.left + nTargWidth;

	//对新图像逐行扫描，通过像素映射完成缩放
	for(i=retRT.top; i<retRT.bottom; i++)
	{
		for(j=retRT.left; j<retRT.right; j++)
		{
			//计算映射的源坐标(最近邻插值)
			nSrc_i = retRT.top + int( (i-retRT.top) / dYScale ) ;
			nSrc_j = retRT.left + int( (j-retRT.left) / dXScale ) ;

			//对应像素赋值
			int nGray = GetGray(nSrc_j, nSrc_i);
			pTo->SetPixel( j, i, RGB(nGray, nGray, nGray) );

		}//for j
	}//for i

	return retRT;
}

/******************* 
void COCRImageProcess::DelScatterNoise(COCRImageProcess* pTo, int lowerThres, int upperThres)
功能：
	去除图像中的离散噪声:滤除大小低于 lowerThres 的连通区域;滤除大小超过 upperThres 的连通区域;其他保留
注：
	只能处理2值图象

参数：
	COCRImageProcess* pTo: 目标图像的 COCRImageProcess 指针
	lowerThres: 下限阈值
	upperThres: 上限阈值
返回值：
	无
********************/
void COCRImageProcess::DelScatterNoise(COCRImageProcess* pTo, int lowerThres, int upperThres)
{
	
	if(upperThres < lowerThres)
	{
		AfxMessageBox("上限阈值必须大于下限阈值！");
		return;
	}

	if(lowerThres < 0)
		lowerThres = 0;
	if(upperThres > 1500)
		upperThres = 1500; //为防止深度递归栈益处，限定upperThres的最大值为1000

	COCRImageProcess image_bkp = *this;
	COCRImageProcess image_res = *pTo;

	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();

	int i,j;
	unsigned char pixel;

	LPBYTE lpVisited = new BYTE[nHeight*nWidth]; //标记该位置是否已被访问过

	for(i=0;i<nHeight*nWidth;i++)
		lpVisited[i] = false; //初始访问标记数组

	int curConnRgnSize = 0; //当前发现的连通区的大小

	
	int nPtArySize = upperThres + 10; //记录访问点坐标数组的大小，是一个不能小于 upperThres 的量
	CPoint* ptVisited;//记录对于连通区的一次探查中访问过的点的坐标
	ptVisited = new CPoint[nPtArySize]; 

	int k = 0;

	for(i=0;i<nHeight;i++)
	{
		for(j=0;j<nWidth;j++)
		{
			
			for(k=0;k<curConnRgnSize;k++)
				lpVisited[ptVisited[k].y*nWidth + ptVisited[k].x] = false;//还原 lpVisited 数组

			curConnRgnSize = 0; //重置为0

			pixel = image_bkp.GetGray(j, i);
			
			if( pixel == 0 ) //找到1个黑像素，进而探查该像素所处的连通区域的大小
			{
				int nRet = TestConnRgn(&image_bkp, lpVisited, nWidth, nHeight, j, i, ptVisited, lowerThres, upperThres, curConnRgnSize);
			
				if( (nRet == 1) || (nRet == -1) ) // >upperThres or <lowerThres
				{
					//滤除
					for(k=0; k<curConnRgnSize; k++)
					{
						image_res.SetPixel(ptVisited[k].x, ptVisited[k].y, RGB(255, 255, 255));
					}
				}
			}

		}// for j
	}// for i

	
	*pTo = image_res;


	delete []lpVisited;
	delete []ptVisited;
}



/******************* 
void COCRImageProcess::ObjectAlign(COCRImageProcess* pTo, vector<RECT> &vecRT)
功能：
	目标对象（字符）的紧缩和对齐，归一化之后使用
注：
	只能处理2值图象

参数：
	COCRImageProcess* pTo: 目标图像的 COCRImageProcess 指针
	vecRT: 表示字符分割的矩形向量数组
	
返回值：
	无
********************/
void COCRImageProcess::ObjectAlign(COCRImageProcess* pTo, vector<RECT> &vecRT)
{
	pTo->InitPixels(255); //目标图像置白

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	int i, j;

	int nSize = vecRT.size(); //取得对象(字符)矩形轮廓数目

	int nNormW, nNormH; //归一化之后的统一矩形宽、高
	//取得归一化之后的统一矩形宽、高
	if( nSize > 0 )
	{
		nNormW = vecRT[0].right - vecRT[0].left + 1;
		nNormH = vecRT[0].bottom - vecRT[0].top + 1;
	}
	else //没有轮廓矩形，直接返回
		return;
	
	int nSrc_i, nSrc_j; //映射源坐标
	RECT rt, rtNew;
	int nMargin = 2; //紧缩对齐后的图像留出左、上边界距

	//依次调整每个轮廓矩形区域的位置
	for(int nObj=0; nObj<nSize; nObj++)
	{
		rt = vecRT[nObj];

		//紧缩对齐后的轮廓矩形，从图像左上角开始，从左至右依次排列
		rtNew.left = nObj * nNormW + nMargin; //左边界
		rtNew.right = (nObj+1) * nNormW + nMargin; //右边界
		rtNew.top = 0 + nMargin; //上边界
		rtNew.bottom = nNormH + nMargin; //下边界

		vecRT[nObj] = rtNew;

		//将原矩形框内的像素映射到新的矩形框中
		for(i=0; i<nNormH; i++)
		{
			for(j=nObj*nNormW; j<(nObj+1)*nNormW; j++)
			{
				//计算映射源坐标
				nSrc_i = rt.top + i;
				nSrc_j = rt.left + j - nObj*nNormW;

				//拷贝像素
				int nGray = GetGray(nSrc_j, nSrc_i);
				pTo->SetPixel(j+nMargin, i+nMargin, RGB(nGray, nGray, nGray));
			}// for j
		}// for i
	}//for nObj

}

















