I = double(imread('umbrellas.jpg'));
P = I(:);
figure(1); clf;
bins = 10;
H = hist(P, bins);
subplot(1, 3, 1); bar(H, 'b');
bins = 20;
H = hist(P, bins);
subplot(1, 3, 2); bar(H, 'b');
bins = 40;
H = hist(P, bins);
subplot(1, 3, 3); bar(H, 'b');