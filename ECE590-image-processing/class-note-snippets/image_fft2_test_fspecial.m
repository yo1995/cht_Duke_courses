clc; clear;
im = imread('cameraman.png');
h = fspecial('gaussian', [51, 51], 3);
% h2 = normrnd(0, 3, [51, 51]);
h = padarray(h, [102, 102], 'both');
s = size(im);
fim = fft2(im);
fh = fft2(h, 256, 256);
fout = fim.*fh;
out = ifft2(fout);
% imagesc(out);axis equal off; colormap gray;

%% reorganize the for quadrants
% out2 = zeros(size(out));
% out2(1:128,1:128) = out(129:256,129:256);
% out2(1:128,129:256) = out(129:256,1:128);
% out2(129:256,1:128) = out(1:128,129:256);
% out2(129:256,129:256) = out(1:128,1:128);

% fftshift just do the swapping of quadrants in 2d. see https://blog.csdn.net/viatorsun/article/details/82387854 
out2 = fftshift(out);
imagesc(out2);axis equal off; colormap gray;
% 将滤波器分成四块重新排列后再次加在图片上