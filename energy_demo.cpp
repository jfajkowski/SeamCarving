#include "energy_demo.h"

extern int edge_type;
int scale = 10;
Mat src, src_gray;
Mat grad;
char* window_name = "Energy Demo";

void energy(int, void*)
{
	int delta = 0;
	int ddepth = CV_16S;
	
	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	if (edge_type == 0)
	{
		Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
		convertScaleAbs(grad_x, abs_grad_x);

		Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
		convertScaleAbs(grad_y, abs_grad_y);

		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	}
	if (edge_type == 1)
	{
		Scharr(src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
		convertScaleAbs(grad_x, abs_grad_x);

		Scharr(src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);
		convertScaleAbs(grad_y, abs_grad_y);

		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	}
	if (edge_type == 2)
	{
		Canny(src_gray, grad, scale, scale * 3, 3);
	}

	imshow(window_name, grad);
}

int showEnergyDemo(Mat src)
{
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// Convert it to gray
	cvtColor(src, src_gray, CV_RGB2GRAY);

	/// Create window
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	/// Create a trackbar
	cvCreateTrackbar("Scale", window_name, &scale, 100, (CvTrackbarCallback)energy);
	cvSetTrackbarPos("Scale", window_name, 100);

	energy(scale, 0);

	waitKey(0);

	return 0;
}
