/**
* @file Morphology_1.cpp
* @brief Erosion and Dilation sample code
* @author OpenCV team
*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Image.h"
#include "BckgSubt.h"
#include "Morf_operations.h"

using namespace std;
using namespace cv;

/// Global variables
vector<Image> images= vector<Image>();
Mat src, src_gray, bckg, bckg_gray, dest;

int main(int, char** argv){

	// Read image
	src = imread("../data/PKlot_FPR05_subset/2013-03-05_08_20_02.jpg");
	cvtColor(src, src_gray, CV_BGR2GRAY);
	//imshow("origin", src);
	//background subtration
	bckg = imread("../data/PKlot_FPR05_subset/2013-03-02_06_30_00.jpg");
	cvtColor(bckg, bckg_gray, CV_BGR2GRAY);
	BckgSubt sub = BckgSubt(src, bckg);
	sub.apply();
	dest = sub.getrmbckg();
	//imshow("sub", dest);

	Morf_operations mf;

	//opening
	Mat g_eroded = mf.Erosion(src_gray,1,4);
	Mat g_dilated = mf.Dilation(g_eroded,1,4);
	imshow("opening gray", g_dilated);


	// Set up the detector with default parameters.
	SimpleBlobDetector detector;

	// Detect blobs.
	vector<KeyPoint> keypoints;
	detector.detect(dest, keypoints);

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	Mat im_with_keypoints;
	drawKeypoints(dest, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	// Show blobs
	//imshow("keypoints", im_with_keypoints);
	
	waitKey(0);
	return 0;
}
