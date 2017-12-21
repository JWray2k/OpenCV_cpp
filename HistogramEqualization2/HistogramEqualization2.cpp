/*
Histograms - 2: Histogram Equalization
http://docs.opencv.org/3.1.0/d5/daf/tutorial_py_histogram_equalization.html (python tutorial)
Goal

In this section,

We will learn the concepts of histogram equalization and use it to improve the contrast of our images.
Theory

Consider an image whose pixel values are confined to some specific range of values only. For eg, brighter 
image will have all pixels confined to high values. But a good image will have pixels from all regions of 
the image. So you need to stretch this histogram to either ends (as given in below image, from wikipedia)
and that is what Histogram Equalization does (in simple words). This normally improves the contrast of the image.

*/