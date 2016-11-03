#include "SURF_detector.h"


int SURF_detector::MIN_HESSIAN = 400;

SURF_detector* SURF_detector::instance = NULL;

vector<KeyPoint> SURF_detector::run(Mat & img)
{
	vector<KeyPoint> res;

	SurfFeatureDetector detector(SURF_detector::MIN_HESSIAN);
	detector.detect(img, res);

	return res;
}

SURF_detector * SURF_detector::getInstance()
{
	if (!SURF_detector::instance) {
		SURF_detector::instance = new SURF_detector();
	}

	return SURF_detector::instance;
}
