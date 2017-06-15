function [ xApprox ] = approx( x, k )
% 用 k 个主成分分量来近似（重建）样本 x
%
% 输入：x --- 原特征空间中的样本，被近似的对象
%       k --- 近似（重建）使用的主分量数目
%
% 输出：xApprox --- 样本的近似（重建）

% 读入 PCA 变换矩阵 V
load Mat/PCA.mat

nLen = length(x);

xApprox=zeros(1, nLen);

for ii = 1:k
    xApprox=xApprox+(x*V(:,ii))*V(:,ii)';
end