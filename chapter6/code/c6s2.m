% c6s2.m

% 读取图片
A = imread('beauty.jpg');
B = imread('cat.jpg');

% 求傅立叶变换
Af = fft2(A);
Bf = fft2(B);

% 分别求幅度谱和相位谱
AfA = abs(Af);
AfB = angle(Af);

BfA = abs(Bf);
BfB = angle(Bf);

% 交换相位谱并重建复数矩阵
AfR = AfA .* cos(BfB) + AfA .* sin(BfB) .* i;
BfR = BfA .* cos(AfB) + BfA .* sin(AfB) .* i;

% 傅立叶反变换
AR = abs(ifft2(AfR));
BR = abs(ifft2(BfR));

% 显示图像
subplot(2,2,1);
imshow(A);
title('美女原图像');

subplot(2,2,2);
imshow(B);
title('猫的原图像');

subplot(2,2,3);
imshow(AR, []);
title('美女的幅度谱和猫的相位谱组合');

subplot(2,2,4);
imshow(BR, []);
title('猫的幅度谱和美女的相位谱组合');
