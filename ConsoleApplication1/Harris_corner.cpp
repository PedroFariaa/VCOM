#include "Harris_corner.h"

vector<cv::KeyPoint> Harris_corner::run(Image img)
{
	vector<cv::KeyPoint> res;

	cv::GoodFeaturesToTrackDetector::Params params;
	params.useHarrisDetector = true;
	params.blockSize = this->BLOCK_SIZE;
	params.qualityLevel = this->k;
	params.minDistance = this->APERTURE_SIZE;

	GoodFeaturesToTrackDetector detector(params);
	detector.detect(img, res);

	return res;
}

Harris_corner* Harris_corner::getInstance()
{
	if (!instance) {
		instance = new Harris_corner();
	}

	return instance;
}
