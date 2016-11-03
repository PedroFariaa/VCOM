#pragma once
#include "Detector.h"


class SURF_detector : public Detector
{
protected:
	static SURF_detector* instance;
public:
	vector<cv::KeyPoint> run(Mat& img);
	static SURF_detector* getInstance();
	static int MIN_HESSIAN;
};

