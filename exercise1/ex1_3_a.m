M = logical(imread('mask.png'));
SE = ones(3);

figure;
subplot(1, 4 , 1);
imagesc(imerode(M, SE)); axis equal; axis tight; title('Erode');
subplot(1, 4, 2);
imagesc(imdilate(M, SE)); axis equal; axis tight; title('Dilate');
subplot(1, 4, 3);
imagesc(imerode(imdilate(M, SE), SE)); axis equal; axis tight; title('Closing');
subplot(1, 4, 4);
imagesc(imdilate(imerode(M, SE), SE)); axis equal; axis tight; title('Opening');
