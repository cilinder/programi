M = logical(imread('mask.png'));
SE = ones(3);

figure;
subplot(1, 4, 1);
imagesc(imerode(M, SE)); axis equal; axis tight; title('Erode');

