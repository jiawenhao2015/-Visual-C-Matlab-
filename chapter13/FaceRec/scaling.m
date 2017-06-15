function [SVFM, lowVec, upVec] = scaling(VecFeaMat, bTest, lRealBVec, uRealBVec)
% Input:  VecFeaMat --- 需要scaling的 m*n 维数据矩阵，每行一个样本特征向量，列数为维数
%         bTest ---  =1：说明是对于测试样本进行scaling，此时必须提供 lRealBVec 和 uRealBVec
%                       的值，此二值应该是在对训练样本 scaling 时得到的
%                    =0：默认值，对训练样本进行 scaling
%         lRealBVec --- n维向量，对训练样本 scaling 时得到的各维的实际下限信息
%         uRealBVec --- n维向量，对训练样本 scaling 时得到的各维的实际上限信息
%
% output: SVFM --- VecFeaMat的 scaling 版本
%         upVec --- 各维特征的上限(只在对训练样本scaling时有意义，bTest = 0)
%         lowVec --- 各维特征的下限(只在对训练样本scaling时有意义，bTest = 0)
if nargin < 2
    bTest = 0;
end

% 缩放目标范围[-1, 1]
lTargB = -1;
uTargB = 1;


[m n] = size(VecFeaMat);

SVFM = zeros(m, n);

if bTest
    if nargin < 4
        error('To do scaling on testset, param lRealB and uRealB are needed.');
    end
    
    if nargout > 1
        error('When do scaling on testset, only one output is supported.');
    end

    for iCol = 1:n
        if uRealBVec(iCol) == lRealBVec(iCol)
            SVFM(:, iCol) = uRealBVec(iCol);
            SVFM(:, iCol) = 0;
        else
            SVFM(:, iCol) = lTargB  +  ( VecFeaMat(:, iCol) - lRealBVec(iCol) ) / ( uRealBVec(iCol)-lRealBVec(iCol) ) * (uTargB-lTargB); % 测试数据的scaling
        end
    end
else
    upVec = zeros(1, n);
    lowVec = zeros(1, n);


    for iCol = 1:n
        lowVec(iCol) = min( VecFeaMat(:, iCol) );
        upVec(iCol) = max( VecFeaMat(:, iCol) );
        if upVec(iCol) == lowVec(iCol)
            SVFM(:, iCol) = upVec(iCol);
            SVFM(:, iCol) = 0;
        else
            SVFM(:, iCol) = lTargB  +  ( VecFeaMat(:, iCol) - lowVec(iCol) ) / ( upVec(iCol)-lowVec(iCol) ) * (uTargB-lTargB); % 训练数据的scaling
        end
    end
end