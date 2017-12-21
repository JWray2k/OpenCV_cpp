/* Matlab source -

function f = energy_1(x)
global image;
Z = image;
Z = im2double(Z);
cform = makecform('srgb2lab');
H = applycform(Z,cform);
%H = rgb2hsv(Z);
global radius;

v=vignet(radius,[x, 0,0,0,0,0]);
mask = v==0;
v(mask) = 1;

%H(:,:,3) = H(:,:,3) ./ v(:,:);
H(:,:,1) = H(:,:,1) ./ v(:,:);

%I=hsv2rgb(H);
cform = makecform('lab2srgb');
I = applycform(H,cform);
G=rgb2gray(I);
G=im2uint8(G);


f = gamma_r(G, 1, 0);
end

*/