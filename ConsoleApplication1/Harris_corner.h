#pragma once
#include "Image.h"
#include "types.hpp"
#include <vector>

using namespace std;
using namespace cv;

class Harris_corner
{
	static Harris_corner* instance;
public:
	//variables
	static int BLOCK_SIZE;
	static int APERTURE_SIZE;
	static int THRESHOLD;
	static double k;

	//functions
	vector<cv::KeyPoint> run(Image img);
	static Harris_corner* getInstance();

};

