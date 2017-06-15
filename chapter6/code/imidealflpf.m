function out = imidealflpf(I, freq)
% imidealflpf函数			构造理想的频域低通滤波器
% I参数				输入的灰度图像
% freq参数				低通滤波器的截止频率
% 返回值：out – 指定的理想低通滤波器
[M,N] = size(I);
out = ones(M,N);
for i=1:M
    for j=1:N
        if (sqrt(((i-M/2)^2+(j-N/2)^2))>freq)
            out(i,j)=0;
        end
    end
end
