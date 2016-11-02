#include "BckgSubt.h"



BckgSubt::BckgSubt(Mat img, Mat bckg)
{
	this->img = img;
	this->bckg = bckg;
}

Mat BckgSubt::getImg()
{
	return img;
}

Mat BckgSubt::getBckg()
{
	return bckg;
}

Mat BckgSubt::getrmbckg()
{
	return rmbckg;
}

void BckgSubt::apply()
{
	subtract(img, bckg, rmbckg);
}

BckgSubt::~BckgSubt()
{
}
