% locateMouth.m

I = imread('mouth.bmp'); %读入图像
Id = im2double(I);
figure, imshow(Id) % 得到8.24(a)
Ibw = im2bw(Id, 0.38); % 以0.38为阈值二值化
Ibw = 1 - Ibw; %为在Matlab中进行处理，将图像反色
figure, imshow(Ibw) % 得到8.24(b)
hold on
[L, num] = bwlabel(Ibw, 8); % 标注连通分量
disp(['图中共有' num2str(num) '个连通分量'])
 
% 找出最大的连通分量（嘴）
max = 0; % 当前最大连通分量的大小
indMax = 0; % 当前最大连通分量的索引
for k = 1:num
    [y x] = find(L == k); % 找出编号为k的连通区的行索引集合y和列索引集合x
    
    nSize = length(y); %计算该连通区中的像素数目
    if(nSize > max)
        max = nSize;
        indMax = k;
    end
end
 
if indMax == 0
    disp('没有找到连通分量')
    return
end
 
% 计算并显示最大连通分量（嘴）的中心
[y x] = find(L == indMax);
yMean = mean(y);
xMean = mean(x);
plot(xMean, yMean, 'Marker', 'o', 'MarkerSize', 14, 'MarkerEdgeColor', 'w', 'MarkerFaceColor', 'w');
plot(xMean, yMean, 'Marker', '*', 'MarkerSize', 12, 'MarkerEdgeColor', 'k'); % 得到8.24(c)
