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
SimpleBlobDetector detector;
int minArea = 1000;
int minThres = 10;
int maxThres = 200;
int minCirc = 0;
int minInert = 0;
Image img1;


void updateBlobParams(int, void*) {
	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;


	// Change thresholds
	params.minThreshold = minThres;
	params.maxThreshold = maxThres;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = minArea;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = minCirc;

	// Filter by Convexity
	params.filterByConvexity = false;
	params.minConvexity = 0.87;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = minInert;

	// Set up the detector with default parameters.
	detector = SimpleBlobDetector::Params(params);

	// Detect blobs.
	std::vector<KeyPoint> keypoints;
	detector.detect(img1.getGreyImage(), keypoints);

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	Mat im_with_keypoints;
	drawKeypoints(img1.getGreyImage(), keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	// Show blobs
	imshow("keypoints", im_with_keypoints);
}


int main(int, char** argv){
	
	img1 = Image(argv[1]);
	images.push_back(img1);
	/*Harris_corner a = Harris_corner();
	a.run(img1);*/


	/// Create windows
	namedWindow("Blob", CV_WINDOW_AUTOSIZE);

	/// Create Area Trackbar
	createTrackbar("Min Area:", "Blob", &minArea, 5000, updateBlobParams);
	/// Create Threshold Trackbars
	createTrackbar("Min Threshold:", "Blob", &minThres, 100, updateBlobParams);
	createTrackbar("Max Threshold:", "Blob", &minThres, 500, updateBlobParams);
	/// Create Circularity Trackbar
	createTrackbar("Min Circularity:", "Blob", &minArea, 10, updateBlobParams);
	/// Create Inertia Trackbar
	createTrackbar("Min Inertia:", "Blob", &minArea, 10, updateBlobParams);

	updateBlobParams(minArea, 0);


	waitKey(0);
	return 0;
}