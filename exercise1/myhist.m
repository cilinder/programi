function [ H , bins ] = myhist ( I , nbins )
I = reshape( I , 1 , numel ( I ) ) ; % r e s h a p e image i n 1D v e c t o r
H = zeros ( 1 , nbins ) ; % i n i t i a l i z e the hi s t o g r am
max_val_in = 255 ; % hi g h e s t i n p u t v al u e
min_val_in = 0 ; % l o w e s t i n p u t v al u e
max_val_out = nbins ; % hi g h e s t c e l l inde x
min_val_out = 1 ; % l o w e s t c e l l inde x
% Compute bin numbers f o r a l l p i x e l s
f = ( max_val_out - min_val_out ) / ( max_val_in - min_val_in );

idx = floor(double(I) * f) + min_val_out;
idx(idx > nbins) = nbins;

for i = 1:length(I)
    H(idx(i)) = H(idx(i)) + 1;
end

H = H / sum(H);

bins = ( (1 : nbins) - min_val_out) ./ f;

