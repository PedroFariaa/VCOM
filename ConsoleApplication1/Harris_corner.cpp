#include "Harris_corner.h"

vector<KeyPoint> Harris_corner::run(Image img)
{
	vector<KeyPoint> res;

	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros(img.getImage().size(), CV_32FC1);

	/// Detector parameters
	this->BLOCK_SIZE = 2;
	this->APERTURE_SIZE = 3;
	this->k = 0.04;

	/// Detecting corners
	cornerHarris(img.getGreyImage(), dst, this->BLOCK_SIZE, this->APERTURE_SIZE, this->k, BORDER_DEFAULT);

	/// Normalizing
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);

	/// Drawing a circle around corners
	for (int j = 0; j < dst_norm.rows; j++)
	{
		for (int i = 0; i < dst_norm.cols; i++)
		{
			if ((int)dst_norm.at<float>(j, i) > this->THRESHOLD)
			{
				circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
			}
		}
	}
	/// Showing the result
	namedWindow("corners_window", CV_WINDOW_AUTOSIZE);
	imshow("corners_window", dst_norm_scaled);

	return res;
}

Harris_corner* Harris_corner::getInstance()
{
	if (!instance) {
		instance = new Harris_corner();
	}

	return instance;
}
