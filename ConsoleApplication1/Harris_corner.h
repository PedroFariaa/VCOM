#pragma once
#include "Image.h"
#include "opencv2/features2d/features2d.hpp"
#include <vector>

using namespace std;
using namespace cv;

class Harris_corner
{
	Harris_corner * instance;
public:
	//variables
	int BLOCK_SIZE = 2;
	int APERTURE_SIZE = 3;
	int THRESHOLD = 0;
	double k = 0.4;

	//functions
	vector<KeyPoint> run(Image img);
	Harris_corner* getInstance();

};

