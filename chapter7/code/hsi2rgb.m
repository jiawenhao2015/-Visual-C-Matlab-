function rgb = hsi2rgb(hsi)
% rgb = hsi2rgb(hsi)把一幅HSI图像转换为RGB图像，
% 其中hsi(:, :, 1)是色度分量，它的范围是除以2*pi后的[0, 1]；
% hsi(:, :, 2)是饱和度分量，范围是[0, 1]；
% hsi(:, :, 3)是亮度分量，范围是[0, 1]。
%
% 输出图像分量：
% rgb(:, :, 1)为红；
% rgb(:, :, 2)为绿；
% rgb(:, :, 3)为蓝。

% 抽取图像分量
hsi = im2double(hsi);
H = hsi(:, :, 1) * 2 * pi;
S = hsi(:, :, 2);
I = hsi(:, :, 3);

% 执行转换方程
R = zeros(size(hsi, 1), size(hsi, 2));
G = zeros(size(hsi, 1), size(hsi, 2));
B = zeros(size(hsi, 1), size(hsi, 2));

% RG扇形(0 <= H < 2*pi/3)
idx = find( (0 <= H) & (H < 2*pi/3));
B(idx) = I(idx) .* (1 - S(idx));
R(idx) = I(idx) .* (1 + S(idx) .* cos(H(idx)) ./ ...
cos(pi/3 - H(idx)));
G(idx) = 3*I(idx) - (R(idx) + B(idx));

% BG扇形(2*pi/3 <= H < 4*pi/3)
idx = find( (2*pi/3 <= H) & (H < 4*pi/3) );
R(idx) = I(idx) .* (1 - S(idx));
G(idx) = I(idx) .* (1 + S(idx) .* cos(H(idx) - 2*pi/3) ./ ...
cos(pi - H(idx)));
B(idx) = 3*I(idx) - (R(idx) + G(idx));

% BR扇形
idx = find( (4*pi/3 <= H) & (H <= 2*pi));
G(idx) = I(idx) .* (1 - S(idx));
B(idx) = I(idx) .* (1 + S(idx) .* cos(H(idx) - 4*pi/3) ./ ...
cos(5*pi/3 - H(idx)));
R(idx) = 3*I(idx) - (G(idx) + B(idx));

% 将3个分量联合成为一个RGB图像
rgb = cat(3, R, G, B);
rgb = max(min(rgb, 1), 0);
