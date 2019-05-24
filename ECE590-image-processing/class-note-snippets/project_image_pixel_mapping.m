%% expand x axis by 2
P = [2, 0, 0; 0, 1, 0; 0, 0, 1];
im = imread('cameraman.png');
tform = maketform('projective', P);
im2 = imtransform(im, tform);
imshow(im2)

%% rotation by 45 degrees
P = [0.7071, -0.7071, 0;
     0.7071, 0.7071, 0;
     0, 0, 1];
im = imread('cameraman.png');
tform = maketform('projective', P);
im2 = imtransform(im, tform);
imshow(im2)

%% some perspective effects
P = [0.7071, -0.7071, 0;
     0.7071, 0.7071, 0.001;
     0, 0, 1];
im = imread('cameraman.png');
tform = maketform('projective', P);
im2 = imtransform(im, tform);
imshow(im2)

%% rectangularize...
% cp2tform()