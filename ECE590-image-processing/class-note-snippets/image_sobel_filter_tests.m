


%% 1D filtering in space domain
x = -5 * pi : 0.01 * pi : 5 * pi;
signal_1d = sinc(x);
% filt = -1 * pi : 0.01 * pi : 1 * pi;
filt = ones(1, 100);

l_x = length(x);
l_f = length(filt);

z = zeros(1, l_x - l_f);
y = myconv1d(signal_1d, filt);

plot(x, y)
% subplot(311)
% plot(x, signal_1d);
% subplot(312)
% plot(filt);
% subplot(313)
% plot(y);

%% 2D filtering
close all; clc
[x, y] = meshgrid(-128:127, -128:127);
% filt_2d = ones(64, 64);
filt_2d = [-1, 0, 1; -2, 0, 2; -1, 0, 1];  % sobel horizontal filter
% sig_2d = sinc(sqrt(x.^2 + y.^2) / (10 * pi));
sig_2d = imread('peppers.png');
sig_2d = rgb2gray(sig_2d);
sig_2d = im2double(sig_2d);
% surf(sig_2d)
% shading interp
figure();
[y_2d, full_y_2d] = myconv2d(sig_2d, filt_2d);
% subplot(211)
% surf(y_2d)
% shading interp
% subplot(212)
% surf(full_y_2d)
% shading interp
subplot(121)
imagesc(y_2d);axis equal off; colormap gray;
subplot(122)
imagesc(myconv2d(y_2d, filt_2d'));axis equal off; colormap gray;
%% 2D matrix using 1D fitering

sobel_x_1 = [1, 2, 1];
sobel_x_2 = [-1, 0, 1];
[y_2d, full_y_2d] = myconv2d(sig_2d, sobel_x_1);
[y_2d, full_y_2d] = myconv2d(full_y_2d, sobel_x_2);
imagesc(y_2d);axis equal off; colormap gray;