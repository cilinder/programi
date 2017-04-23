I = rgb2gray(imread('bird.jpg'));
treshold = otsu(I);
M = I > treshold;

SE = ones(17);

C = imerode(imdilate(M, SE), SE);

imshow(C);