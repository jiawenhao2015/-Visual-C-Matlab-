function rgb = yuv2rgb(yuv)
% yuv = rgb2yuv(rgb)把一幅RGB图像转换为YUV图像，
% 输入图像是一个彩色像素的M×N×3的数组，
% 其中每一个彩色像素都在特定空间位置的彩色图像中对应红、绿、蓝三个分量。
% 假如所有的RGB分量是均衡的，那么HSI转换就是未定义的。
% 输入图像可能是double（取值范围是[0, 1]），uint8或 uint16。
%
% 输出YUV图像是uint8。
yuv = im2double(yuv);
y = yuv(:, :, 1);
u = yuv(:, :, 2);
v = yuv(:, :, 3);

% 执行转换函数
r = y + 1.402*v;
g = y - 0.344*u - 0.714*v;
b = y + 1.772*u;

% 防止溢出
if(r < 0)
    r = 0;
end;
if(r > 1.0)
    r = 1.0;
end;
if(g < 0)
    g = 0;
end;
if(g > 1.0)
    g = 1.0;
end;
if(b < 0)
    b = 0;
end;
if(b > 1.0)
    b = 1.0;
end;
    
% 联合rgb
r = r*255;
g = g*255;
b = b*255;
rgb = cat(3, r, g, b);
rgb = uint8(rgb);
