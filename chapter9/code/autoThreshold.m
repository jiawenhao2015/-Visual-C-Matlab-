function [Ibw, thres] = autoThreshold(I)
% 迭代法自动阈值分割
%
% 输入：I - 要进行自动阈值分割的灰度图像
% 输出：Ibw - 分割后的二值图像
%      thres - 自动分割采用的阈值

thres = 0.5 * (double(min(I(:))) + double(max(I(:)))); %初始阈值
done = false; %结束标志
while ~done
	g = I >= thres;
	Tnext = 0.5 * (mean(I(g)) + mean(I(~g)));
	done = abs(thres - Tnext) < 0.5;
	thres = Tnext;
end;

Ibw = im2bw(I, thres/255); % 二值化