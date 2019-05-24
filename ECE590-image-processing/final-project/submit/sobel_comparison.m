% initialization
clear;
clc;
miu = 0;
sigma = 15;
RMSE = @(x,y)(1/length(x(:))*sum((x(:)-y(:)).^2))^0.5;
PSNR = @(x,y)20*log10(max(y(:))/RMSE(x,y));

% load images, resize to 64*64
im_brick = rgb2gray(imread('brick.png'));
S_brick = double(imresize(im_brick, [64, 64]));

im_brick_brightness = rgb2gray(imread('brick_brightness.png'));
S_brick_brightness = double(imresize(im_brick_brightness, [64, 64]));

im_brick_brightness2 = rgb2gray(imread('brick_brightness_2.png'));
S_brick_brightness2 = double(imresize(im_brick_brightness2, [64, 64]));

im_cameraman = imread('cameraman.png');
S_cameraman = double(imresize(im_cameraman(21:148, 81:208), [64, 64]));

im_pill_part = rgb2gray(imread('pill_part.png'));
S_pill_part = double(imresize(im_pill_part, [64, 64]));

im_lena = rgb2gray(imread('lena_std.tif'));
S_lena = double(imresize(im_lena, [64, 64]));

% the noise
N = normrnd(miu,sigma,size(S_cameraman));

S = S_cameraman;
figure();
imagesc(S);
colormap gray; axis equal tight off;
title('Noise free image')

I = S + N;
figure();
imagesc(I);
colormap gray; axis equal tight off;
title('Noisy image')

filt_2d_x = [-1, 0, 1; -2, 0, 2; -1, 0, 1];  % sobel horizontal filter
filt_2d_y = [-1, 0, 1; -2, 0, 2; -1, 0, 1];  % sobel vertical filter

im1 = imfilter(I, filt_2d_x, 'replicate');
im1 = imfilter(im1, filt_2d_y, 'replicate');

figure();
imagesc(im1);
colormap gray; axis equal tight off;
RMSE1 = RMSE(S, im1);
PSNR1 = PSNR(im1, S);
title(['Sobel with RMSE = ', num2str(RMSE1), ' PSNR = ', num2str(PSNR1)]);
