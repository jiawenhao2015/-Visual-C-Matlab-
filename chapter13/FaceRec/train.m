function train(C, gamma)
% 整个训练过程，包括读入图像，PCA降维以及多类 SVM 训练，各个阶段的处理结果分别保存至文件：
%   将 PCA 变换矩阵 W 保存至 Mat/PCA.mat
%   将 scaling 的各维上、下界信息保存至 Mat/scaling.mat
%   将 PCA 降维并且 scaling 后的数据保存至 Mat/trainData.mat
%   将多类 SVM 的训练信息保存至 Mat/multiSVMTrain.mat

global imgRow;
global imgCol;

display(' ');
display(' ');
display('训练开始...');

nPerson=40;
nFacesPerPerson = 5;
display('读入人脸数据...');
[imgRow,imgCol,FaceContainer,faceLabel]=ReadFaces(nFacesPerPerson,nPerson);
display('..............................');


nFaces=size(FaceContainer,1);%样本（人脸）数目

display('PCA降维...');
[pcaFaces, W] = fastPCA(FaceContainer, 20); % 主成分分析PCA
% pcaFaces是200*20的矩阵, 每一行代表一张主成分脸(共40人，每人5张)，每个脸20个维特征
% W是分离变换矩阵, 10304*20 的矩阵
visualize_pc(W);%显示主成分脸
display('..............................');

X = pcaFaces;

display('Scaling...');
[X,A0,B0] = scaling(X);
save('Mat/scaling.mat', 'A0', 'B0');

% 保存 scaling 后的训练数据至 trainData.mat
TrainData = X;
trainLabel = faceLabel;
save('Mat/trainData.mat', 'TrainData', 'trainLabel');
display('..............................');



for iPerson = 1:nPerson
    nSplPerClass(iPerson) = sum( (trainLabel == iPerson) );
end

multiSVMStruct = multiSVMTrain(TrainData, nSplPerClass, nPerson, C, gamma);
display('正在保存训练结果...');
save('Mat/multiSVMTrain.mat', 'multiSVMStruct');
display('..............................');
display('训练结束。');