function displayImage(v,row,col)
% 向量 v 为一幅图像按列存储得到的行向量，函数 displayImage(...) v 转化成原始的 row * col 图像，并显示之
%
% 输入：v --- 一幅图像按列存储得到的行向量
%      row --- 原始图像的行数
%      col --- 原始图像的列数

[m n] = size(v);


if m ~= 1
   error('v必须为行向量');
end;
if  row*col ~= n
   error('图像尺寸与输入向量 v 的维数不符');
end;

% 原始图像I
I=zeros(row,col);
I(:)=v(:);


figure;
imshow(I, []);

