function Icorr = imcorr(I, w)
% function Icorr = imcorr(I, w, )
% 计算图像 I 与子模式 w 的相关响应，并提示最大的响应位置
% 
% Input：I - 原始图像
%        w - 子图像
%
% Output：Icorr - 响应图像

[m, n] = size(I);
[m0, n0] = size(w);

Icorr = zeros(m-m0+1, n-n0+1); %为响应图像分配空间

vecW = double( w(:) ); %按列存储为向量
normW = norm(vecW); %模式图像对应向量的模

for ii = 1:m-m0+1
    for jj = 1:n-n0+1
        subMat = I(ii:ii+m0-1, jj:jj+n0-1);
        vec = double( subMat(:) ); %按列存储为向量
        Icorr(ii, jj) = vec' * vecW / (norm(vec)*normW+eps); %计算当前位置的相关
    end
end

% 找到最大响相应位置
[iMaxRes, jMaxRes] = find(Icorr == max( Icorr(:) ) );
figure, imshow(I);
hold on
for ii = 1:length(iMaxRes)
    plot(jMaxRes(ii), iMaxRes(ii), 'w*');
    plot([jMaxRes(ii), jMaxRes(ii)+n0-1], [iMaxRes(ii), iMaxRes(ii)], 'w-' );
    plot([jMaxRes(ii)+n0-1, jMaxRes(ii)+n0-1], [iMaxRes(ii), iMaxRes(ii)+m0-1], 'w-' );
    plot([jMaxRes(ii), jMaxRes(ii)+n0-1], [iMaxRes(ii)+m0-1, iMaxRes(ii)+m0-1], 'w-' );
    plot([jMaxRes(ii), jMaxRes(ii)], [iMaxRes(ii), iMaxRes(ii)+m0-1], 'w-' );
end