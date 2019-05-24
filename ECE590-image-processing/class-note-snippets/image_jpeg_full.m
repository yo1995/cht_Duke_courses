% C:\Chen\Software\Study\MATLAB\2017a\toolbox\images\imdata

%% quantization table
Q = [16 11 10 16 24 40 51 61 ;
     12 12 14 19 26 28 60 55 ;
     14 13 16 24 40 57 69 56 ;
     14 17 22 29 51 87 80 62 ;
     18 22 37 56 68 109 103 77 ;
     24 35 55 64 81 104 113 92 ;
     49 64 78 87 103 121 120 101;
     72 92 95 98 112 100 103 99];
Q = Q.*5;
RMSE = @(x,y)(1/length(x(:))*sum((x(:)-y(:)).^2))^0.5;
 
%% test image 
I = imresize(imread('saturn.png'), 0.5);
ImageSize = 8*numel(I);
Y_d = rgb2ycbcr( I );
% Downsample:
Y_d(:,:,2) = 2*round(Y_d(:,:,2)/2);
Y_d(:,:,3) = 2*round(Y_d(:,:,3)/2);
% DCT compress:
A = zeros(size(Y_d));
B = A;
for channel = 1:3
    for j = 1:8:size(Y_d,1)-7
        for k = 1:8:size(Y_d,2)-7
            II = Y_d(j:j+7,k:k+7,channel);
            freq = dct2(II);
            freq = Q.*round(freq./Q);
            A(j:j+7,k:k+7,channel) = freq;
            % do the inverse at the same time:
            B(j:j+7,k:k+7,channel) = idct2(freq);
        end
    end
end
b = A(:);
b = b(:);
b(b==0)=[];  %remove zeros.
b = floor(255*(b-min(b))/(max(b)-min(b)));
symbols = unique(b);
prob = histcounts(b,length(symbols))/length(b);
dict = huffmandict(symbols, prob);
enco = huffmanenco(b, dict);
FinalCompressedImage = length(enco);
I_f = ycbcr2rgb(uint8(B));

disp(['final compression rate is ' num2str(ImageSize/ FinalCompressedImage)  'x']);

subplot(1,2,1)
imshow(I)
title('Original')
subplot(1,2,2)
imshow(I_f);
title('Compressed')
rmse = RMSE(I, I_f);

disp(['RMSE is ' num2str(rmse)]);
