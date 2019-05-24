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

S = S_brick_brightness2;
figure();
imagesc(S);
colormap gray; axis equal tight off;
title('Noise free image')

I = S + N;
figure();
imagesc(I);
colormap gray; axis equal tight off;
title('Noisy image')

x = 1:1000:10001;
y1 = zeros(1, length(x));
y2 = zeros(1, length(x));
for i = 1:1:11
    Jnlm1 = nonlocalMeansFast(I, [5,5], x(i));  % nonlocalMeansFastSqrt, nonlocalMeansSlow
    figure();
    imagesc(Jnlm1);
    colormap gray; axis equal tight off;
    RMSE1 = RMSE(S, Jnlm1);
    PSNR1 = PSNR(Jnlm1, S);
    y1(i) = RMSE1;
    y2(i) = PSNR1;
    title(['Original NLM with RMSE = ', num2str(RMSE1), ' PSNR = ', num2str(PSNR1)]);
end

%% RMSE plot
figure();
plot(x,y1,x,y2, 'linewidth', 2);
axis([1, 10001, 0, 35])
axis square;box on;
xlabel('sigma for weight function')
legend('RMSE', 'PSNR')