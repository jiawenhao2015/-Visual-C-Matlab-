function out = imfreqfilt(I, ff)
% imfreqfilt函数			对灰度图像进行频域滤波
% 参数I				输入的空域图像
% 参数ff				应用的与原图像等大的频域滤镜

if (ndims(I)==3) && (size(I,3)==3)   % RGB图像
    I = rgb2gray(I);
end

if (size(I) ~= size(ff))
    msg1 = sprintf('%s: 滤镜与原图像不等大，检查输入', mfilename);
    msg2 = sprintf('%s: 滤波操作已经取消', mfilename);
    eid = sprintf('Images:%s:ImageSizeNotEqual',mfilename);
    error(eid,'%s %s',msg1,msg2);
end

% 快速傅立叶变换
f = fft2(I);

% 移动原点
s = fftshift(f);

% 应用滤镜及反变换
out = s .* ff; %对应元素相乘实现频域滤波
out = ifftshift(out);
out = ifft2(out);

% 求模值
out = abs(out);

% 归一化以便显示
out = out/max(out(:));
