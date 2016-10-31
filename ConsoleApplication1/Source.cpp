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
#include "Harris_corner.h"

using namespace std;
using namespace cv;

/// Global variables
vector<Image> images= vector<Image>();


int main(int, char** argv){
	
	Image img1 = Image(argv[1]);
	images.push_back(img1);
	Harris_corner a = Harris_corner();
	a.run(img1);
	waitKey(0);
	return 0;
}
