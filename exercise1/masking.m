%First the bird

Bird = imread('bird.jpg');

imshow(Bird);
title('Default loaded image');
pause(4); 
%close;

Bird_bw = rgb2gray(Bird);

imshow(Bird_bw);
title('Transformed to black and white');
pause(4); %close;

Bird_t = otsu(Bird_bw);
Bird_mask = Bird_bw > Bird_t;

imshow(Bird_mask);
title('Black and white bird after tresholding');
pause(4); %close;

SE = ones(17);
Bird_mask_closed = imerode(imdilate(Bird_mask, SE), SE);

imshow(Bird_mask_closed);
title('Mask after closing operation');
pause(4); %close;

Bird_masked = immask(Bird, Bird_mask_closed);

colormap default; imagesc(Bird_masked);
title('After applying the mask on the original image');
pause(4);
%close;

%Now for the eagle

Eagle = imread('eagle.jpg');

imshow(Eagle);
title('Eagle');
pause(4); %close;

Eagle_bw = rgb2gray(Eagle);

imshow(Eagle_bw);
title('Black and white');
pause(4); %close;

Eagle_treshold = otsu(Eagle_bw);
Eagle_mask_over = Eagle_bw > Eagle_treshold;
Eagle_mask_under = Eagle_bw < Eagle_treshold;

colormap gray;
subplot(1, 2, 1); imagesc(Eagle_mask_over);
title('Pixels over treshold');
subplot(1, 2, 2); imagesc(Eagle_mask_under);
title('Pixels under treshold');
pause(4); %close;

SE = ones(15);
Eagle_mask_over_closed = imdilate(imerode(Eagle_mask_over, SE), SE);
Eagle_mask_under_opened = imerode(imdilate(Eagle_mask_under, SE), SE);

colormap gray;
subplot(1, 2, 1); imagesc(Eagle_mask_over_closed);
title('Mask(over) closed');
subplot(1, 2, 2); imagesc(Eagle_mask_under_opened);
title('Mask(under) opened');
pause(4);
close;

Eagle_masked = immask(Eagle, Eagle_mask_under_opened);

colormap default; imagesc(Eagle_masked);
title('Applied mask');
pause(4);
close;
