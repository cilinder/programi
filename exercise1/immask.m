function I_masked = immask(I, B_mask)


R = double(I(:, :, 1));
G = double(I(:, :, 2));
B = double(I(:, :, 3));

R_m = zeros(size(R));
G_m = zeros(size(G));
B_m = zeros(size(B));

R_m(B_mask==1) = R(B_mask==1);
G_m(B_mask==1) = G(B_mask==1);
B_m(B_mask==1) = B(B_mask==1);

I_masked = uint8(cat(3, R_m, G_m, B_m));

end