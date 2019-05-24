%% blur added
clc; clear; close;
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

%% wiener filter
N = randn(s) * 1;
sigma = 1;
fn = fft2(N);
x = linspace(-128, 128, 256) * 1e-2;
[x, y] = meshgrid(x, x);

ff = 1./ (x.^2 + y.^2 + 1);

imagesc(ff);axis equal off; colormap gray;
ENN = sigma^2;
EFF = ff;
W = (fh') ./ (fh'* fh + ENN ./ EFF);

fout = fim .* W;

out1 = ifft2(fout);
out2 = fftshift(out1);
imagesc(abs(out1));axis equal off; colormap gray;
