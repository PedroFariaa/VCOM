#pragma once
#include "opencv2\core.hpp"

using namespace cv;


class BckgSubt
{
private:
	Mat img, bckg, rmbckg;

public:
	BckgSubt(Mat img, Mat bckg);
	Mat getImg();
	Mat getBckg();
	Mat getrmbckg();
	void apply();
	~BckgSubt();
};

