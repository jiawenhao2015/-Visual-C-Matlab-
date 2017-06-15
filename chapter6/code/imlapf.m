function out = imlapf(I)
% imlapff函数			构造频域拉普拉斯滤波器
% I参数				输入的灰度图像

[M,N] = size(I);
out = ones(M,N);
for i=1:M
    for j=1:N
        out(i,j) = -((i-M/2)^2+(j-N/2)^2);
    end
end
