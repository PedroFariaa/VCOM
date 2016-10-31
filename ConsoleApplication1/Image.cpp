#include "Image.h"



Image::Image()
{
	this->n_cars = 0;
}

Image::Image(char* filename)
{
	src = imread(filename);
	cvtColor(src, gray, CV_BGR2GRAY);
}
/*
void Image::showImage()
{
	imshow("Image", src);
}


void Image::showgreyImage()
{
	imshow("Grey Image", gray);
}*/

Mat Image::getImage()
{
	return this->src;
}

cv::Mat Image::getGreyImage()
{
	return this->gray;
}

Image::~Image()
{
}
