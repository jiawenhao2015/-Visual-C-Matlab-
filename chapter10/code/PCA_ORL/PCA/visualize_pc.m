function visualize_pc(E)
% 显示主成分分量（主成分脸，即变换空间中的基向量）
%
% 输入：E --- 矩阵，每一列是一个主成分分量


[size1 size2] = size(E);
global imgRow;
global imgCol;
row = imgRow;
col = imgCol;

if size2 ~= 20
   error('只用于显示 20 个主成分');
end;

figure
img = zeros(row, col);
for ii = 1:20
    img(:) = E(:, ii);
    subplot(4, 5, ii);
    imshow(img, []);
end

