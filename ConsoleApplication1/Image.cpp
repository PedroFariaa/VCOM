#include "Image.h"



Image::Image()
{
	this->n_cars = 0;
}

Image::Image(char* filename)
{
	this->src = imread(filename);
	cvtColor(src, this->gray, CV_BGR2GRAY);
}

Image::showImage()
{
	imshow("Image", this->src);
}


Image::showgreyImage()
{
	imshow("Grey Image", this->gray);
}

cv::Mat Image::getImage()
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
