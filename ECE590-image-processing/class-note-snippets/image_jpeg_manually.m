%% load the image
clc; clear;
S = (imread('cameraman.png'));  % im2double
blocks = mat2cell(S, ones(32,1).*8, ones(32,1).*8);

%% cosine transform, something like ft w/o complex num.

% quantization table
Q = [16 11 10 16 24 40 51 61 ;
     12 12 14 19 26 28 60 55 ;
     14 13 16 24 40 57 69 56 ;
     14 17 22 29 51 87 80 62 ;
     18 22 37 56 68 109 103 77 ;
     24 35 55 64 81 104 113 92 ;
     49 64 78 87 103 121 120 101;
     72 92 95 98 112 100 103 99];
 
 for i = 1:32
    for j = 1:32
        blocks{i, j} = idct2(Q.*round(dct2(blocks{i, j}) ./ Q));
    end
 end

S2 = cell2mat(blocks);
subplot(121)
imagesc(S);axis equal off; colormap gray;
subplot(122)
imagesc(S2);axis equal off; colormap gray;
