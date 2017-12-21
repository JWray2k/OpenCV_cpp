/* Matlab code - http://pages.cs.wisc.edu/~flynn/cs638_project_files/m_files/correct.m

% Patrick Flynn
% CS 638 Spring 2009
% Final Project - Vignetting Correction
clc; clear all; close all;


filename = 'Penguins_2.jpg';
info = imfinfo(filename);
if(strcmp(info.ColorType,'indexed'))
[I, map] = imread(filename);
I = ind2rgb(I,map);
elseif(strcmp(info.ColorType,'grayscale'))
I = imread(filename);
I(:,:,2) = I(:,:,1);
I(:,:,3) = I(:,:,1);
else
I = imread(filename);
end
global image;
image = I;
[rows, cols, lay] = size(I);
r = zeros(rows, cols);
xc = ceil(cols/2);
yc = ceil(rows/2);
for u=1:rows
for v=1:cols
r(u,v) = sqrt( abs(v-xc)^2 + abs(u-yc)^2);
end
end
global radius;
radius = r;

[x,fval,exitflag,output] = fminbnd(@energy_1, 0, 10^5);
x = fminsearch(@energy,[x,0,0,0,0,0]);

*/