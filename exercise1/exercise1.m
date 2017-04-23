A = imread('umbrellas.jpg');
Ad = double(A);
A_gray = uint8((Ad(:, :, 1) + Ad(:, :, 2) + Ad(:, :, 3)) / 3);
Ar = floor(A_gray ./ 4);
imagesc(Ar);
imshow(Ar);