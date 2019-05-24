%% gaussian filter
% clc; clear;
% im = imread('cameraman.png');
% h = fspecial('gaussian', [51, 51], 3);
% h = padarray(h, [102, 102], 'both');
% s = size(im);
% fim = fft2(im);
% fh = fft2(h, 256, 256);
% fout = fim.*fh;
% out = ifft2(fout);
% out2 = fftshift(out);
% imagesc(out2);axis equal off; colormap gray;

%% blur added
clc; clear;
im = imread('cameraman.png');
h = fspecial('motion', 10, 45);
h = padarray(h, [123, 123], 'both');
s = size(im);
fim = fft2(im);
fh = fft2(h, 256, 256);
fout = fim.*fh;
out = ifft2(fout);
out = fftshift(out);

% out = conv2(im, h, 'same');
imagesc(out);axis equal off; colormap gray;

%% restore blur
fim = fft2(out);
fh = fft2(h, 256, 256);
fout = fim ./ fh;
out1 = ifft2(fout);
out2 = fftshift(out1);
imagesc(out2);axis equal off; colormap gray;

%% add some noise and test the filter again
HS = out;
I = HS + randn(s) * 0.1;
hlow = fspecial('gaussian', [51, 51], 3);
hlow = padarray(hlow, [102, 102], 'both');
fhlow = fft2(h, 256, 256);

cut = ones(s);
cut(127:129, 127:129) = 0;
% cut(125:131, 125:131) = 0;
cut = fftshift(cut);
fcut = fft2(cut);

imagesc(I);axis equal off; colormap gray;
fim = fft2(I);
fh = fft2(h, 256, 256);
fout = fim ./ fh .* fcut;  % .* fhlow
out1 = ifft2(fout);
out2 = fftshift(out1);
imagesc(-out2);axis equal off; colormap gray;

