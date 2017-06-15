% compensate.m
% 彩色补偿
im=double(imread('plane.bmp'));
subplot(1,2,1);
imshow(uint8(im));
title('原始图');
[m,n,p]=size(im);
[h1,k1]=min(255-im(:,:,1)+im(:,:,2)+im(:,:,3));
[j1,minx]=min(h1);
 i1=k1(j1);%提取图像中最接近红色的点，其在im中的坐标为i1,j1
 r1=im(i1,j1,1);
 g1=im(i1,j1,2);
 b1=im(i1,j1,3);
R=0.30*r1+0.59*g1+0.11*b1;

[h2,k2]=min(255-im(:,:,2)+im(:,:,1)+im(:,:,3));
[j2,minx]=min(h2);
 i2=k2(j2);%提取图像中最接近绿色的点，其在im中的坐标为i2,j2
 r2=im(i2,j2,1);
 g2=im(i2,j2,2);
 b2=im(i2,j2,3);
G=0.30*r2+0.59*g2+0.11*b2;

[h3,k3]=min(255-im(:,:,3)+im(:,:,1)+im(:,:,2));
[j3,minx]=min(h3);
 i3=k3(j3);%提取图像中最接近蓝色的点，其在im中的坐标为i3,j3
 r3=im(i3,j3,1);
 g3=im(i3,j3,2);
 b3=im(i3,j3,3);
B=0.30*r3+0.59*g3+0.11*b3;

A1=[r1 r2 r3
    g1 g2 g3
    b1 b2 b3];
A2=[R 0 0
    0 G 0
    0 0 B];
C=A1*inv(A2);

for i=1:m
    for j=1:n

          imR=im(i,j,1);
          imG=im(i,j,2);
          imB=im(i,j,3);
          temp=inv(C)*[imR;imG;imB];
          S(i,j,1)=temp(1);
          S(i,j,2)=temp(2);
          S(i,j,3)=temp(3);
    end
end
S=uint8(S);
subplot(1,2,2);
imshow(S);
title('补偿后');
