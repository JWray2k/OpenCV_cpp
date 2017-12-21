/* Matlab code - http://pages.cs.wisc.edu/~flynn/cs638_project_files/m_files/correct_vignetting.m

% Patrick Flynn
% CS 638 Spring 2009
% Final Project - Vignetting Correction
clc; clear all; close all;


filename = 'Penguins.jpg';
info = imfinfo(filename);
if (strcmp(info.ColorType, 'indexed'))
[I, map] = imread(filename);
I = ind2rgb(I, map);
elseif(strcmp(info.ColorType, 'grayscale'))
I = imread(filename);
I(:, : , 2) = I(:, : , 1);
I(:, : , 3) = I(:, : , 1);
else
I = imread(filename);
end
global image;
image = I;
I = im2uint8(I);
G = rgb2gray(I);

subplot(1, 2, 2);
gamma_r(G, 1, 1)
subplot(1, 2, 1), imshow(I);

pause;
close(1);

filename = 'Penguins_1.jpg';
info = imfinfo(filename);
if (strcmp(info.ColorType, 'indexed'))
[I, map] = imread(filename);
I = ind2rgb(I, map);
elseif(strcmp(info.ColorType, 'grayscale'))
I = imread(filename);
I(:, : , 2) = I(:, : , 1);
I(:, : , 3) = I(:, : , 1);
else
I = imread(filename);
end
image = I;
I = im2uint8(I);
G = rgb2gray(I);

subplot(1, 2, 2);
gamma_r(G, 1, 1)
subplot(1, 2, 1), imshow(I);

pause;
close(1);

filename = 'Penguins_2.jpg';
info = imfinfo(filename);
if (strcmp(info.ColorType, 'indexed'))
[I, map] = imread(filename);
I = ind2rgb(I, map);
elseif(strcmp(info.ColorType, 'grayscale'))
I = imread(filename);
I(:, : , 2) = I(:, : , 1);
I(:, : , 3) = I(:, : , 1);
else
I = imread(filename);
end
image = I;
I = im2uint8(I);
G = rgb2gray(I);

subplot(1, 2, 2);
gamma_r(G, 1, 1)
subplot(1, 2, 1), imshow(I);
*/