I = imread('peppers.png');
I = rgb2gray(I);

spec = fft2(I);
spec = fftshift(spec);

% imagesc(log(abs(spec)))
% colormap gray
surf(abs(spec))


[X, Y] = meshgrid(1:256, 1:256);
I = sin((X + Y) / pi);
imagesc(I);
% colormap gray

spec = fft2(I);
spec = fftshift(spec) .* 0.001;
surf(abs(spec))