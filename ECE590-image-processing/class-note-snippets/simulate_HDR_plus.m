clear;close;
%% simulate the low light condition with various exposure time
im = imread('cameraman.png');
S = im2double(im).*256;

miu = 0;
sigma = 5;
s = size(im);
N = normrnd(miu,sigma,s);

% g for gain, t for exposure time
g = 10;
t = 0.1;
I = g*(S*t+N);
% imagesc(I);axis equal off;colormap gray;
I_long = 0.1*g*(S*10*t+N);

loop_count = 10;
y = zeros(s(1), s(2), 'double');

%% compose a less noisy image with multiple short exposure shots
for i = 1:loop_count
    N_loop = normrnd(miu,sigma,s);
    y = y + g*(S*t+N_loop);
end

y = 1/loop_count * y;

%% compare RMSE for 10 shot and long exposure
RMSE = @(x,y)(1/length(x(:))*sum((x(:)-y(:)).^2))^0.5;
RMSE(I, S)
RMSE(y, S)
RMSE(I_long, S)

%% draw the images
figure();
imagesc(I);axis equal off;colormap gray;
figure();
imagesc(I_long);axis equal off;colormap gray;
figure();
imagesc(y);axis equal off;colormap gray;