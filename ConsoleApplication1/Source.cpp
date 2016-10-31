/**
* @file Morphology_1.cpp
* @brief Erosion and Dilation sample code
* @author OpenCV team
*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Image.h"

using namespace std;
using namespace cv;

/// Global variables
vector<Mat> images= vector<Mat>();


int main(int, char** argv){
	images.push_back(Image(argv[1]));


	//show the image on window 
	imshow("Image1", src);

	cvtColor(src, imgg, CV_BGR2GRAY);
	imshow("Greyscale", imgg);

	//equalized histogram
	equalizeHist(imgg, imgeh);
	imshow("eq histogram", imgeh);

	waitKey(0);
	return 0;
}
