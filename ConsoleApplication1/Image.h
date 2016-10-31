#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

class Image{
private:
	Mat src, gray;
	int n_cars;
public:
	Image();
	Image(char* filename);
	//void showImage();
	//void showgreyImage();
	Mat getImage();
	Mat getGreyImage();
	~Image();
};

