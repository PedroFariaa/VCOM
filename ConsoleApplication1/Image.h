#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

class Image{
private:
	cv::Mat src, gray;
	int n_cars;
public:
	Image();
	Image(char* filename);
	void showImage();
	void showgreyImage();
	cv::Mat getImage();
	cv::Mat getGreyImage();
	~Image();
};

