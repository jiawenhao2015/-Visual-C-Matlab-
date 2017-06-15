% GUIOpenFaceImage.m

global filepath;
[filename, pathname] = uigetfile({'*.pgm;*.jpg;*tif', '(*.pgm), (*.jpg), (*.tif)'; ...
   '*.*', 'All Files(*.*)' }, 'Select a face image to be recognized');
if filename~=0
    filepath = [pathname,filename];
    axes(h_axes1);
    imshow(imread(filepath));
end
