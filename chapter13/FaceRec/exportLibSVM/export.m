function export(strMat, strLibSVM)
% 将以参数 strMat 指定的文件中的数据导出为能够被 LibSVM 使用的格式，生成的文件名由参数 strLibSVM 指定
%
% 输入：strMat --- 源文件名（包括路径），'.mat'文件，默认为'../Mat/trainData.mat'，其中必须包含训练数据
%                  TrainData 和类标签 trainLabel，该文件可在训练 SVM 过程中生成
%       strLibSVM --- 目标文件名（包括路径），'.txt'文件，默认为'trainData.txt'

if nargin < 1
    strMat = '../Mat/trainData.mat';
    strLibSVM = 'trainData.txt';
elseif nargin < 2
    strLibSVM = 'trainData.txt';
end

[fid, fMsg ] = fopen(strLibSVM, 'w'); % 建立目标输出文件
if fid == -1
    disp(fMsg );
    return
end

strNewLine = [13 10]; % 换行
strBlank = ' ';
    
load(strMat)

[nSamp, nDim] = size( TrainData );


for iSamp = 1:nSamp
    fwrite(fid, num2str(trainLabel(iSamp)), 'char');
    
    for iDim = 1:nDim
        fwrite(fid, strBlank, 'char');
        fwrite(fid, [num2str(iDim) ':'], 'char');
        fwrite(fid, num2str(TrainData(iSamp, iDim)), 'char');        
    end
    
    fwrite(fid, strNewLine, 'char');
end



fclose(fid);

