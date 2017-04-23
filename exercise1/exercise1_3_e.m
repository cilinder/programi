I = imread('coins.jpg');

BW = rgb2gray(I);

treshold = otsu(BW);

M = BW < treshold;

imshow(M);
title('Applied treshold');
pause(2); close;

SE = ones(9);
M_closed = imerode(imdilate(M, SE), SE);

imshow(M_closed);
title('Mask after closing operation');
pause(2); close;

L = bwlabel(M_closed);

label_max = max(L(:));

for i = 1: label_max
    if i ~= 1
        L(L==i) = 0;
    end;
end;

subplot(1, 2, 1);
imshow(I); title('Original');
subplot(1, 2, 2);
imshow(immask(I, L > 0)); title('Processed');
colormap gray;