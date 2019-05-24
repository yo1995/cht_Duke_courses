%% initialization
% clear;
% clc;
miu = 0;
sigma = 15;
RMSE = @(x,y)(1/length(x(:))*sum((x(:)-y(:)).^2))^0.5;
PSNR = @(x,y)20*log10(max(y(:))/RMSE(x,y));

%% load images, resize to 64*64
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
%% section 2, weight graph demo

% also need to comment out the weight graph plot in each function to get
% the graphs

% Jnlm = nonlocalMeansSlow(S_brick_brightness2, [5,5], 5e4);
% Jnlm = nonlocalMeansWeightAvg(S_brick_brightness2, [5,5], 5e4);
% Jnlm = nonlocalMeansWeightPeak(S_brick_brightness2, [5,5], 5e4);
% Jnlm = nonlocalMeansSqrtDist(S_brick_brightness2, [5,5], 5e4);

%% section 3, processing

h_square = 3e3;  % 3e3 for cameraman, 2e3 for lena, 3e3 for brick

S = S_brick_brightness2;  % S_cameraman;
% figure();
% imagesc(S);
% colormap gray; axis equal tight off;
% title('Noise free image')

I = S + N;
% figure();
% imagesc(I);
% colormap gray; axis equal tight off;
% title('Noisy image')

Jnlm1 = nonlocalMeansSlow(I, [5,5], h_square);  % nonlocalMeansFast
figure();
imagesc(Jnlm1);
colormap gray; axis equal tight off;
RMSE1 = RMSE(S, Jnlm1);
PSNR1 = PSNR(Jnlm1, S);
title(['Original NLM with RMSE = ', num2str(RMSE1), ' PSNR = ', num2str(PSNR1)]);

% Jnlm2 = nonlocalMeansWeightAvg(I, [5,5], h_square);
% figure();
% imagesc(Jnlm2);
% colormap gray; axis equal tight off;
% RMSE2 = RMSE(S, Jnlm2);
% PSNR2 = PSNR(Jnlm2, S);
% title(['Avg NLM with RMSE = ', num2str(RMSE2), ' PSNR = ', num2str(PSNR2)]);

% Jnlm3 = nonlocalMeansWeightPeak(I, [5,5], h_square);
% figure();
% imagesc(Jnlm3);
% colormap gray; axis equal tight off;
% RMSE3 = RMSE(S, Jnlm3);
% PSNR3 = PSNR(Jnlm3, S);
% title(['Norm NLM with RMSE = ', num2str(RMSE3), ' PSNR = ', num2str(PSNR3)]);

% Jnlm4 = nonlocalMeansSqrtDist(I, [5,5], h_square);
% figure();
% imagesc(Jnlm4);
% colormap gray; axis equal tight off;
% RMSE4 = RMSE(S, Jnlm4);
% PSNR4 = PSNR(Jnlm4, S);
% title(['SQRT NLM with RMSE = ', num2str(RMSE4), ' PSNR = ', num2str(PSNR4)]);
