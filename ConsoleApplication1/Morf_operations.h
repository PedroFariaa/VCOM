#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


using namespace cv;

class Morf_operations
{
private:
	// 0: Rect | 1: Cross | 2: Ellipse
	int erosion_elem = 1;
	//limited to 21
	int erosion_size = 4;
	// 0: Rect | 1: Cross | 2: Ellipse
	int dilation_elem = 1;
	//limited to 21
	int dilation_size = 4;
	//return images
	Mat erosion_dst, dilation_dst;
public:
	//apply dilation to image
	Mat Dilation(Mat src);
	Mat Dilation(Mat src, int erosion_elem, int erosion_size);
	//apply erosion to image
	Mat Erosion(Mat src);
	Mat Erosion(Mat src, int dilation_elem, int dilation_size);
};

