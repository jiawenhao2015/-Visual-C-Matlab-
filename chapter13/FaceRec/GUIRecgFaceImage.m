% GUIRecgFaceImage.m

nClass = classify(filepath);
msgbox( ['所属类别为:',num2str(nClass)] );
axes(h_axes2);
f = imread(['Data/ORL/S',num2str(nClass),'/1.pgm']); % 打开该人的第1幅图像
imshow(f);