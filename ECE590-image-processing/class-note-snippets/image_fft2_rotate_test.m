% I = imread('cameraman.png');
I = imread('plate.png');
% [X, Y] = meshgrid(1:256, 1:256);
% I = sin((X + Y) / pi);

theta = 45;
c = cos(theta);
s = sin(theta);

f_I = fft2(I);
J = I;
% for i = 1:size(I,1)
%     for j = 1:size(I,2)
%         J(i, j) = I((i*c - j*s), (i*s + j*c));
%     end
% end

J = imrotate(I, theta, 'crop');
% imagesc(J);axis equal off; colormap gray;
figure();
subplot(121)
imagesc(I);axis equal off; colormap gray;
subplot(122)
imagesc(J);axis equal off; colormap gray;

f_J = fft2(J);

figure();
subplot(121);
imagesc(log(abs(f_I)));axis equal off; colormap gray;
subplot(122);
imagesc(log(abs(f_J)));axis equal off; colormap gray;
