#include "Morf_operations.h"



Mat Morf_operations::Erosion(Mat src)
{
	int erosion_type;
	if (this->erosion_elem == 0) { erosion_type = MORPH_RECT; }
	else if (this->erosion_elem == 1) { erosion_type = MORPH_CROSS; }
	else if (this->erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(erosion_type,
		Size(2 * this->erosion_size + 1, 2 * this->erosion_size + 1),
		Point(this->erosion_size, this->erosion_size));

	/// Apply the erosion operation
	erode(src, erosion_dst, element);

	//return eroded image
	return erosion_dst;
}

Mat Morf_operations::Erosion(Mat src, int e_elem, int e_size)
{
	int erosion_type;
	if (e_elem == 0) { erosion_type = MORPH_RECT; }
	else if (e_elem == 1) { erosion_type = MORPH_CROSS; }
	else if (e_elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(erosion_type,
		Size(2 * e_size + 1, 2 * e_size + 1),
		Point(e_size, e_size));

	/// Apply the erosion operation
	erode(src, erosion_dst, element);

	//return eroded image
	return erosion_dst;
}


Mat Morf_operations::Dilation(Mat src)
{
	int dilation_type;
	if (this->dilation_elem == 0) { dilation_type = MORPH_RECT; }
	else if (this->dilation_elem == 1) { dilation_type = MORPH_CROSS; }
	else if (this->dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(dilation_type,
		Size(2 * this->dilation_size + 1, 2 * this->dilation_size + 1),
		Point(this->dilation_size, this->dilation_size));
	/// Apply the dilation operation
	dilate(src, dilation_dst, element);

	return dilation_dst;
}

Mat Morf_operations::Dilation(Mat src, int dilation_elem, int dilation_size)
{
	int dilation_type;
	if (dilation_elem == 0) { dilation_type = MORPH_RECT; }
	else if (dilation_elem == 1) { dilation_type = MORPH_CROSS; }
	else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(dilation_type,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));
	/// Apply the dilation operation
	dilate(src, dilation_dst, element);
	
	return dilation_dst;
}
