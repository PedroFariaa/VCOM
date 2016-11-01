#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


using namespace cv;

class Morf_operations
{
private:
	// EROSION
	int erosion_elem = 1; // 0: Rect | 1: Cross | 2: Ellipse
	int erosion_size = 4; //limited to 21

	// DILATION
	int dilation_elem = 1; // 0: Rect | 1: Cross | 2: Ellipse
	int dilation_size = 4; //limited to 21

	// TOPHAT
	int morph_elem = 1; // 0: Rect | 1: Cross | 2: Ellipse
	int morph_size = 4;

	//return images
	Mat erosion_dst, dilation_dst;
public:
	//apply dilation to image
	Mat Dilation(Mat src);
	Mat Dilation(Mat src, int erosion_elem, int erosion_size);
	//apply erosion to image
	Mat Erosion(Mat src);
	Mat Erosion(Mat src, int dilation_elem, int dilation_size);
	//top-hat
	Mat TopHat(Mat src);
	//black-hat
	Mat BlackHat(Mat scr);
};

