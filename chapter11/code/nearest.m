% 例11.3 利用最小距离分类器分类３种鸢尾属植物
load fisheriris %载入Matlab自带的鸢尾属植物数据集

% 每类的前40个样本用于生成代表该类的模板，后10个作为独立的测试样本
m1 = mean( meas(1:40, :) ); %第1类的前40个样本的平均向量
m2 = mean( meas(51:90, :) ); %第2类的前40个样本的平均向量
m3 = mean( meas(101:140, :) ); %第3类的前40个样本的平均向量

% 测试样本集
Test = [meas(41:50, :); meas(91:100, :); meas(141:150, :)];
% 测试样本集对应的类别标签
classLabel(1:10) = 1;
classLabel(11:20) = 2;
classLabel(21:30) = 3;

% 利用最小距离分类器分类测试样本
class = zeros(1, 30); %类标签
for ii = 1:size(Test, 1)
   d(1) = norm(Test(ii, :) - m1); %与第１类的距离
   d(2) = norm(Test(ii, :) - m2); %与第２类的距离
   d(3) = norm(Test(ii, :) - m3); %与第３类的距离
   
   [minVal class(ii)] = min(d); %计算最小距离并将距离样本最短的类赋给类标签数组 class
end

% 测试最小距离分类器的识别率
nErr = sum(class ~= classLabel);
rate = 1 - nErr / length(class);
strOut = ['识别率为', num2str(rate*100), '%']


