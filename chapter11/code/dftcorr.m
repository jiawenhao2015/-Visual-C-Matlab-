function Icorr = dftcorr(I, w)
% function Icorr = dftcorr(I, w)
% 在频域下计算图像 I 与子模式 w 的相关响应，并提示最大的响应位置
% 
% Input：I - 原始图像
%        w - 子图像
%
% Output：Icorr - 响应图像
I = double(I);
[m n] = size(I);
[m0 n0] = size(w);
F = fft2(I);
w = conj(fft2(w, m, n)); %w 频谱的共轭
Ffilt = w .* F; %频域滤波结果
Icorr = real(ifft2(Ffilt)); %反变换回空域


% 找到最响相应位置
[iMaxRes, jMaxRes] = find(Icorr == max( Icorr(:) ) );
figure, imshow(I, []);
hold on
for ii = 1:length(iMaxRes)
    plot(jMaxRes(ii), iMaxRes(ii), 'w*');
    plot([jMaxRes(ii), jMaxRes(ii)+n0-1], [iMaxRes(ii), iMaxRes(ii)], 'w-' );
    plot([jMaxRes(ii)+n0-1, jMaxRes(ii)+n0-1], [iMaxRes(ii), iMaxRes(ii)+m0-1], 'w-' );
    plot([jMaxRes(ii), jMaxRes(ii)+n0-1], [iMaxRes(ii)+m0-1, iMaxRes(ii)+m0-1], 'w-' );
    plot([jMaxRes(ii), jMaxRes(ii)], [iMaxRes(ii), iMaxRes(ii)+m0-1], 'w-' );
end