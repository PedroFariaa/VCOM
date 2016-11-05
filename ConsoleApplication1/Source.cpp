/**
* @file Morphology_1.cpp
* @brief Erosion and Dilation sample code
* @author OpenCV team
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>

#include <stdio.h>
#include <tchar.h>


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

Mat src, src_gray, bckg, bckg_gray, dest;
int n_spots = 0;

/// Global variables
vector<Image> images= vector<Image>();

bool comparator(Point2f a, Point2f b) {
	return a.x<b.x;
}
void sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center)
{
	std::vector<cv::Point2f> top, bot;
	for (int i = 0; i < corners.size(); i++)
	{
		if (corners[i].y < center.y)
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}
	sort(top.begin(), top.end(), comparator);
	sort(bot.begin(), bot.end(), comparator);
	cv::Point2f tl = top[0];
	cv::Point2f tr = top[top.size() - 1];
	cv::Point2f bl = bot[0];
	cv::Point2f br = bot[bot.size() - 1];
	corners.clear();
	corners.push_back(tl);
	corners.push_back(tr);
	corners.push_back(br);
	corners.push_back(bl);
}

cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
	int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
	if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
	{
		cv::Point2f pt;
		pt.x = ((x1*y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4)) / d;
		pt.y = ((x1*y2 - y1*x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3*x4)) / d;
		//-10 is a threshold, the POI can be off by at most 10 pixels
		if (pt.x<min(x1, x2) - 10 || pt.x>max(x1, x2) + 10 || pt.y<min(y1, y2) - 10 || pt.y>max(y1, y2) + 10) {
			return Point2f(-1, -1);
		}
		if (pt.x<min(x3, x4) - 10 || pt.x>max(x3, x4) + 10 || pt.y<min(y3, y4) - 10 || pt.y>max(y3, y4) + 10) {
			return Point2f(-1, -1);
		}
		return pt;
	}
	else
		return cv::Point2f(-1, -1);
}

int main(int, char** argv){

	// Read image
	src = imread("../data/FEUP/teste.jpg");
	Size size(600, 800);
	resize(src, src, size);
	//src = imread("../data/PKlot_FPR05_subset/2013-03-05_08_20_02.jpg");
	cvtColor(src, src_gray, CV_BGR2GRAY);
	//imshow("origin", src);


	//Apply blur to smooth edges and use adapative thresholding  
	Size size2(3, 3);
	GaussianBlur(src, src, size2, 0);
	//adaptiveThreshold(src, adaptiveTresholded, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 75, 15);
	//aptiveThreshold(src, src, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 75, 10);
	//bitwise_not(src, src);

	//vector<Vec4i> lines;
	//HoughLinesP(src, lines, 1, 2 * CV_PI / 180, 100, 1000, 50);

	imshow("clean up", src);


	Canny(src, src, 100, 200, 3);
	/// Find contours   
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(src, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	

	/// Draw contours
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		//drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	vector<Rect> boundRect;
	for (int i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		boundRect.push_back(boundingRect(contours[i]));
		if (boundRect[i].area() > 5000 && boundRect[i].area() < 20000) {
			rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
			n_spots++;
			cout << "Lugares totais: " << n_spots;
		}
		
	}


	imshow("Result window", drawing);

	Mat dst, color_dst;
	Canny(src, dst, 50, 200, 3);
	cvtColor(dst, color_dst, CV_GRAY2BGR);

	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 80, 30, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		
		line(color_dst, Point(lines[i][0], lines[i][1]),
			Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 3, 8);
	}


	Mat img2;
	cvtColor(src, img2, CV_GRAY2RGB);
	int* poly = new int[lines.size()];
	for (int i = 0; i<lines.size(); i++)poly[i] = -1;
	int curPoly = 0;
	vector<vector<cv::Point2f> > corners;
	for (int i = 0; i < lines.size(); i++)
	{
		for (int j = i + 1; j < lines.size(); j++)
		{

			cv::Point2f pt = computeIntersect(lines[i], lines[j]);
			if (pt.x >= 0 && pt.y >= 0 && pt.x<img2.size().width&&pt.y<img2.size().height) {

				if (poly[i] == -1 && poly[j] == -1) {
					vector<Point2f> v;
					v.push_back(pt);
					corners.push_back(v);
					poly[i] = curPoly;
					poly[j] = curPoly;
					curPoly++;
					continue;
				}
				if (poly[i] == -1 && poly[j] >= 0) {
					corners[poly[j]].push_back(pt);
					poly[i] = poly[j];
					continue;
				}
				if (poly[i] >= 0 && poly[j] == -1) {
					corners[poly[i]].push_back(pt);
					poly[j] = poly[i];
					continue;
				}
				if (poly[i] >= 0 && poly[j] >= 0) {
					if (poly[i] == poly[j]) {
						corners[poly[i]].push_back(pt);
						continue;
					}

					for (int k = 0; k<corners[poly[j]].size(); k++) {
						corners[poly[i]].push_back(corners[poly[j]][k]);
					}

					corners[poly[j]].clear();
					poly[j] = poly[i];
					continue;
				}
			}
		}
	}

	for (int i = 0; i<corners.size(); i++) {
		cv::Point2f center(0, 0);
		if (corners[i].size()<4)continue;
		for (int j = 0; j<corners[i].size(); j++) {
			center += corners[i][j];
		}
		center *= (1. / corners[i].size());
		sortCorners(corners[i], center);
	}

	Mat img3;
	cvtColor(src, img3, CV_GRAY2RGB);


	for (int i = 0; i<corners.size(); i++) {
		if (corners[i].size()<4)continue;
		Rect r = boundingRect(corners[i]);
		if (r.area()<50000)continue;
		cout << r.area() << endl;
		// Define the destination image  
		cv::Mat quad = cv::Mat::zeros(r.height, r.width, CV_8UC3);
		// Corners of the destination image  
		std::vector<cv::Point2f> quad_pts;
		quad_pts.push_back(cv::Point2f(0, 0));
		quad_pts.push_back(cv::Point2f(quad.cols, 0));
		quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
		quad_pts.push_back(cv::Point2f(0, quad.rows));
		// Get transformation matrix  
		cv::Mat transmtx = cv::getPerspectiveTransform(corners[i], quad_pts);
		// Apply perspective transformation  
		cv::warpPerspective(img3, quad, transmtx, quad.size());
		stringstream ss;
		ss << i << ".jpg";
		imshow(ss.str(), quad);
	}
	


	///bounding
	/*
	Mat imgHSV;
	cvtColor(src, imgHSV, CV_BGR2HSV);
	////////////////////
	Mat blur_out;
	GaussianBlur(imgHSV, blur_out, Size(1, 1), 2.0, 2.0);

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	Mat range_out;
	inRange(blur_out, Scalar(100, 100, 100), Scalar(120, 255, 255), range_out);

	/// Draw polygonal contour + bonding rects
	Mat drawing3 = Mat::zeros(range_out.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(255, 0, 255);
		drawContours(drawing3, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing3, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
	}
	*/


	namedWindow("Source", 1);
	imshow("Source", src);

	namedWindow("Detected Lines", 1);
	imshow("Detected Lines", color_dst);



	/*
	//background subtration
	Mat srcb = imread("../data/PKlot_FPR05_subset/2013-03-09_08_50_03.jpg");
	bckg = imread("../data/PKlot_FPR05_subset/2013-03-09_07_15_01.jpg");
	cvtColor(bckg, bckg_gray, CV_BGR2GRAY);
	//BckgSubt sub = BckgSubt(src_gray, bckg_gray);
	//sub.apply();
	//dest = sub.getrmbckg();
	//imshow("sub", dest);
	*/

	/*
	Morf_operations mf;

	//opening
	Mat g_eroded = mf.Erosion(src_gray,1,2);
	Mat g_dilated = mf.Dilation(g_eroded,1,2);
	imshow("opening gray", g_dilated);
	//para os carros pretos
	Mat EH;
	equalizeHist(g_dilated, EH);
	imshow("equa hist", EH);

	Mat gbh = mf.BlackHat(src_gray);
	*/


	/*
	BckgSubt sub2 = BckgSubt(g_dilated, bckg_gray);
	sub2.apply();
	Mat dest2 = sub.getrmbckg();
	imshow("sub2", dest2);
	*/



	/*
	Canny(g_dilated, g_dilated, 100, 200, 3);
	/// Find contours   
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(g_dilated, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// Draw contours
	Mat drawing = Mat::zeros(g_dilated.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	imshow("Result window", drawing);
	*/


	/*
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

	
	*/

	/*
	Mat gbh = mf.BlackHat(src_gray);
	imshow("gray blackhat", gbh);
	*/

	/*
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
	*/

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