clear;close;
S = double(imread('cameraman.png'));

%% see correlation
% im2 = imtranslate(im, [15, 5]);
% xcorr = imfilter(im, im2);
% figure();
% [X, Y] = meshgrid(1:256);
% surf(X, Y, xcorr);

%% generate shifted images
s = size(S);
y = zeros(s(1), s(2), 'double');

miu = 0;
sigma = 5;
g = 10;
t = 0.1;
N = normrnd(miu,sigma,s);
I = g*(S*t+N);

loop_count = 10;
%% compose a less noisy image with multiple short exposure shots
for i = 1:loop_count
    N_loop = normrnd(miu,sigma,s);
    shift1 = 20 * randn() - 10;
    shift2 = 20 * randn() - 10;
    tmp = imtranslate(g*(S*t+N_loop), [shift1, shift2]);
    y = y + tmp;
end

y = 1/loop_count * y;
RMSE = @(x,y)(1/length(x(:))*sum((x(:)-y(:)).^2))^0.5;
RMSE(I, S)
RMSE(y, S)

figure();
imagesc(y);axis equal off;colormap gray;
