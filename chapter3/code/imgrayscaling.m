function out = imgrayscaling(varargin)
% IMGRAYSCALING     执行灰度拉伸功能
%   语法：
%       out = imgrayscaling(I, [x1,x2], [y1,y2]);
%       out = imgrayscaling(X, map, [x1,x2], [y1,y2]);
%       out = imgrayscaling(RGB, [x1,x2], [y1,y2]);
%   这个函数提供灰度拉伸功能，输入图像应当是灰度图像，但如果提供的不是灰度
%   图像的话，函数会自动将图像转化为灰度形式。x1，x2，y1，y2应当使用双精度
%   类型存储，图像矩阵可以使用任何MATLAB支持的类型存储。

[A, map, x1 , x2, y1, y2] = parse_inputs(varargin{:});

% 计算输入图像A中数据类型对应的取值范围
range = getrangefromclass(A);
range = range(2);

% 如果输入图像不是灰度图，则需要执行转换
if ndims(A)==3,% A矩阵为3维，RGB图像
  A = rgb2gray(A);
elseif ~isempty(map),% MAP变量为非空，索引图像
  A = ind2gray(A,map);
end % 对灰度图像则不需要转换
 
% 读取原始图像的大小并初始化输出图像
[M,N] = size(A);
I = im2double(A);		% 将输入图像转换为双精度类型
out = zeros(M,N);
 
% 主体部分，双级嵌套循环和选择结构
for i=1:M
    for j=1:N
        if I(i,j)<x1
            out(i,j) = y1 * I(i,j) / x1;
        elseif I(i,j)>x2
            out(i,j) = (I(i,j)-x2)*(range-y2)/(range-x2) + y2;
        else
            out(i,j) = (I(i,j)-x1)*(y2-y1)/(x2-x1) + y1;
        end
    end
end

% 将输出图像的格式转化为与输入图像相同
if isa(A, 'uint8') % uint8
    out = im2uint8(out);
elseif isa(A, 'uint16')
    out = im2uint16(out);
% 其它情况，输出双精度类型的图像
end

 % 输出:
if nargout==0 % 如果没有提供参数接受返回值
  imshow(out);
  return;
end
%-----------------------------------------------------------------------------
function [A, map, x1, x2, y1, y2] = parse_inputs(varargin);
% 这就是用来分析输入参数个数和有效性的函数parse_inputs
% A       输入图像，RGB图 (3D), 灰度图 (2D), 或者索引图 (X)
% map     索引图调色板 (:,3)
% [x1,x2] 参数组 1，曲线中两个转折点的横坐标
% [y1,y2] 参数组 2，曲线中两个转折点的纵坐标
% 首先建立一个空的map变量，以免后面调用isempty(map)时出错
map = [];
 
%   IPTCHECKNARGIN(LOW,HIGH,NUM_INPUTS,FUNC_NAME) 检查输入参数的个数是否
%   符合要求，即NUM_INPUTS中包含的输入变量个数是否在LOW和HIGH所指定的范围
%   内。如果不在范围内，则此函数给出一个格式化的错误信息。
iptchecknargin(3,4,nargin,mfilename);
 
%   IPTCHECKINPUT(A,CLASSES,ATTRIBUTES,FUNC_NAME,VAR_NAME, ARG_POS) 检查给定
%   矩阵A中的元素是否属于给定的类型列表。如果存在元素不属于给定的类型，则给出
%   一个格式化的错误信息。
iptcheckinput(varargin{1},...
              {'uint8','uint16','int16','double'}, ...
              {'real', 'nonsparse'},mfilename,'I, X or RGB',1);
 
switch nargin
 case 3 %            可能是imgrayscaling(I, [x1,x2], [y1,y2]) 或 imgrayscaling(RGB, [x1,x2], [y1,y2])
  A = varargin{1};
  x1 = varargin{2}(1);
  x2 = varargin{2}(2);
  y1 = varargin{3}(1);
  y2 = varargin{3}(2);
 case 4
  A = varargin{1};%               imgrayscaling(X, map, [x1,x2], [y1,y2])
  map = varargin{2};
  x1 = varargin{2}(1);
  x2 = varargin{2}(2);
  y1 = varargin{3}(1);
  y2 = varargin{3}(2);
end

% 检测输入参数的有效性
% 检查RGB数组
if (ndims(A)==3) && (size(A,3)~=3)   
    msg = sprintf('%s: 真彩色图像应当使用一个M-N-3维度的数组', ...
                  upper(mfilename));
    eid = sprintf('Images:%s:trueColorRgbImageMustBeMbyNby3',mfilename);
    error(eid,'%s',msg);
end
 
if ~isempty(map) 
% 检查调色板
  if (size(map,2) ~= 3) || ndims(map)>2
    msg1 = sprintf('%s: 输入的调色板应当是一个矩阵', ...
                   upper(mfilename));
    msg2 = '并拥有三列';
    eid = sprintf('Images:%s:inColormapMustBe2Dwith3Cols',mfilename);
    error(eid,'%s %s',msg1,msg2);
    
  elseif (min(map(:))<0) || (max(map(:))>1)
    msg1 = sprintf('%s: 调色板中各个分量的强度 ',upper(mfilename));
    msg2 = '应当在0和1之间';
    eid = sprintf('Images:%s:colormapValsMustBe0to1',mfilename);
    error(eid,'%s %s',msg1,msg2);
  end
end
 
% 将int16类型的矩阵转换成uint16类型
if isa(A,'int16')
  A = int16touint16(A);
end
