clear;close;
S = double(imread('cameraman.png'));
%% generate shifted images
s = size(S);
y = zeros(s(1), s(2), 'double');

miu = 0;
sigma = 5;
g = 10;
t = 0.1;
N = normrnd(miu,sigma,s);
I = g*(S*t+N);
I1 = imtranslate(I, [15, 5]);

loop_count = 10;
y = I1;
%% compose a less noisy image with multiple short exposure shots
for i = 1:loop_count - 1
    N_loop = normrnd(miu,sigma,s);
%     shift1 = round(20 * rand() - 10);
%     shift2 = round(20 * rand() - 10);
    shift1 = 0;
    shift2 = 0;
    tmp = imtranslate(g*(S*t+N_loop), [shift1, shift2]);
    
    xcorr = imfilter(I1, tmp);
%     [~ ,ind] = max(xcorr);
    
    maximum = max(max(xcorr));
    [X, Y] = find(xcorr==maximum);
    tmp = imtranslate(tmp, [X-128, Y-128]);
    y = y + tmp;
end

y = 1/loop_count * y;
RMSE = @(x,y)(1/length(x(:))*sum((x(:)-y(:)).^2))^0.5;
RMSE(I, S)
RMSE(y, S)

figure();
imagesc(y);axis equal off;colormap gray;
