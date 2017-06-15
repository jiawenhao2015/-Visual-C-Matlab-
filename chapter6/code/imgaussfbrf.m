function out = imgaussfbrf(I, freq, width)
% imidealflpf函数 		构造频域高斯带阻滤波器
% I参数				输入的灰度图像
% freq参数				阻带中心频率
% width参数			阻带宽度

[M,N] = size(I);
out = ones(M,N);
for i=1:M
    for j=1:N
        out(i,j) = 1-exp(-0.5*((((i-M/2)^2+(j-N/2)^2)-freq^2)/(sqrt(i.^2+j.^2)*width))^2);
    end
end
