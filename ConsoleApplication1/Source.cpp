/**
* @file Morphology_1.cpp
* @brief Erosion and Dilation sample code
* @author OpenCV team
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Image.h"
#include "BckgSubt.h"
#include "Morf_operations.h"
#include "Detector.h"
#include "SURF_detector.h"

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


	Morf_operations mf;

	//opening
	Mat g_eroded = mf.Erosion(src_gray,1,4);
	Mat g_dilated = mf.Dilation(g_eroded,1,4);
	imshow("opening gray", g_dilated);

	Mat gbh = mf.BlackHat(src_gray);

	SurfFeatureDetector detectorS(800);

	vector<KeyPoint> keypoints_1, keypoints_2, keypoints_3;

	detectorS.detect(src_gray, keypoints_1);
	detectorS.detect(g_dilated, keypoints_2);
	detectorS.detect(gbh, keypoints_3);

	//-- Draw keypoints
	Mat img_keypoints_1; Mat img_keypoints_2; Mat img_keypoints_3;

	drawKeypoints(src_gray, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(g_dilated, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(gbh, keypoints_3, img_keypoints_3, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	//-- Show detected (drawn) keypoints
	imshow("Keypoints 1", img_keypoints_1);
	imshow("Keypoints 2", img_keypoints_2);
	imshow("Keypoints 3", img_keypoints_3);

	// STEP 2
	SurfDescriptorExtractor extractor;
	Mat descriptors_1, descriptors_2, descriptors_3;

	extractor.compute(src_gray, keypoints_1, descriptors_1);
	extractor.compute(g_dilated, keypoints_2, descriptors_2);
	extractor.compute(gbh, keypoints_3, descriptors_3);

	


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
	/*
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
	createTrackbar("Min Circularity:", "Blob", &minCirc, 10, updateBlobParams);
	/// Create Inertia Trackbar
	createTrackbar("Min Inertia:", "Blob", &minInert, 10, updateBlobParams);

	updateBlobParams(minArea, 0);
	*/

	waitKey(0);
	return 0;
}