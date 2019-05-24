%% original image
im = imread('coins.png');

%% gaussian filter
h = fspecial('gaussian', [21, 21], 3);
im2 = imfilter(im, h);
imagesc(im2);
axis equal off tight; colormap gray;

%% median filter, cannot represent as a convolution
im3 = medfilt2(im, [10, 10]);
imagesc(im3);
axis equal off tight; colormap gray;

%% add some scattering noise to the image
im4 = im;
u = ceil(rand(100)*numel(size(im4)));
im4(u) = 0;
u = ceil(rand(100)*numel(size(im4)));
im4(u) = 255;

imagesc(im4);
axis equal off tight; colormap gray;
im4 = medfilt2(im4, [5, 5]);
figure();
imagesc(im4);
axis equal off tight; colormap gray;