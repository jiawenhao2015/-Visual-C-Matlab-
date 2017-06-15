function face=ReadAFace(dir)

% 根据指定目录读入一个人脸
% 
% 输入:  --dir:保存人脸图像的路径
% 
% 输出:   --face:列向量,其中img的像素按列存储

img=imread(dir);
[imgRow,imgCol]=size(img);
face=zeros(1, imgRow*imgCol);

face = img(:)';
