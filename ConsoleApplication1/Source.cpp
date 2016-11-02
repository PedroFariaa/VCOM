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

	Mat th = mf.TopHat(src);
	Mat bh = mf.BlackHat(src);
	Mat gth = mf.TopHat(src_gray);
	Mat gbh = mf.BlackHat(src_gray);
	/*
	imshow("tophat", th);
	imshow("blackhat", bh);
	imshow("gray tophat", gth);
	*/
	imshow("gray blackhat", gbh);

	//closing
	Mat eroded_gbh = mf.Erosion(gbh, 1, 1);
	Mat final_gbh = mf.Dilation(eroded_gbh, 1, 1);
	imshow("after closing", final_gbh);
	imshow("corwees", src);
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
	

	/* BLOB  */
	img1 = Image(argv[1]);
	images.push_back(img1);

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