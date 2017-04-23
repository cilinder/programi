function Closed = imclose(I)
SE = ones(17);
Closed = imerode(imdilate(I, SE), SE);
end