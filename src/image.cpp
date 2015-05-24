#include "image.h"

Image::Image(const Mat &src, std::string n)
{
	name = n;
	height = src.rows;
	width = src.cols;
	image = src;
	duplicate = src;
	calculateEnergy();
	calculateCumulatedEnergy();
}

void Image::calculateEnergy()
{
	///Find edges
	int delta = 0;
	int ddepth = CV_16S;

	/// Generate grad_x and grad_y
	Mat src_gray;
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	//GaussianBlur(image, src_gray, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// Convert it to gray
	cvtColor(image, src_gray, CV_RGB2GRAY);

	if (edge_type == 0)
	{
		Sobel(src_gray, grad_x, ddepth, 1, 0, 3, 1, delta, BORDER_DEFAULT);
		convertScaleAbs(grad_x, abs_grad_x);

		Sobel(src_gray, grad_y, ddepth, 0, 1, 3, 1, delta, BORDER_DEFAULT);
		convertScaleAbs(grad_y, abs_grad_y);

		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, energy);
	}
	if (edge_type == 1)
	{
		Scharr(src_gray, grad_x, ddepth, 1, 0, 1, delta, BORDER_DEFAULT);
		convertScaleAbs(grad_x, abs_grad_x);

		Scharr(src_gray, grad_y, ddepth, 0, 1, 1, delta, BORDER_DEFAULT);
		convertScaleAbs(grad_y, abs_grad_y);

		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, energy);
	}
	if (edge_type == 2)
	{
		Canny(src_gray, energy, 20, 20 * 3, 3);
	}
}

void Image::calculateCumulatedEnergy()
{
	energy.convertTo(cumulated_energy, CV_32S);

	// Find rest of values
	for (int y = 1; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int min = cumulated_energy.at<int>(y - 1, x);
			if (x > 0 && cumulated_energy.at<int>(y - 1, x - 1) < min) 
			{
				min = cumulated_energy.at<int>(y - 1, x - 1);
			}
			if (x < width - 1 && cumulated_energy.at<int>(y - 1, x + 1) < min) 
			{
				min = cumulated_energy.at<int>(y - 1, x + 1);
			}
			cumulated_energy.at<int>(y, x) = min + (int)energy.at<unsigned char>(y, x);
		}
	}
}

void Image::calculatePixelEnergy(int x, int y)
{
	// Calculate gradient function for single pixel
	Vec3b l, r, u, d;
	if (x > 0) l = image.at<Vec3b>(y, x - 1); else l = { 255, 255, 255 };
	if (x < width-1) r = image.at<Vec3b>(y, x + 1); else r = { 255, 255, 255 };
	if (y > 0) u = image.at<Vec3b>(y - 1, x); else u = { 255, 255, 255 };
	if (y < height-1) d = image.at<Vec3b>(y + 1, x); else d = { 255, 255, 255 };
	
	unsigned int val = (l[0] - r[0])*(l[0] - r[0]) + (l[1] - r[1])*(l[1] - r[1]) + (l[2] - r[2])*(l[2] - r[2]) +
		(u[0] - d[0])*(u[0] - d[0]) + (u[1] - d[1])*(u[1] - d[1]) + (u[2] - d[2])*(u[2] - d[2]);

	if (val < 255)
		energy.at<unsigned char>(y, x) = (unsigned char)val;
	else 
		energy.at<unsigned char>(y, x) = 255;
}

void Image::findSeam()
{
	int min_x = 0;
	paths.resize(1);
	paths.at(0).reserve(height);
	//Look for the end of seam with minimal energy
	for (int x = 1; x < width - 1; x++)
	{
		if (cumulated_energy.at<int>(height - 1, x) < cumulated_energy.at<int>(height - 1, min_x))
			min_x = x;
	}

	

	//Find the rest of the seam
	int x = min_x;
	paths.at(0).push_back(x);
	for (int y = height - 2; y >= 0; y--)
	{
		if (x > 0 && cumulated_energy.at<int>(y, x - 1) < cumulated_energy.at<int>(y, x))
			x--;

		if (x < width - 1 && cumulated_energy.at<int>(y, x + 1) < cumulated_energy.at<int>(y, x))
			x++;

		paths.at(0).push_back(x);
	}
	found_seams++;
	std::reverse(paths.at(0).begin(), paths.at(0).end());
}

void Image::deleteSeam(bool flag)
{
	if (!flag)
	{
		// Paint seam red
		for (int y = 0; y < height; y++)
		{
			image.at<Vec3b>(y, paths.at(0).at(y)) = { 0, 0, 255 };
		}
	}
	else
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = paths.at(0).at(y); x < width - 1; x++)
			{
				image.at<Vec3b>(y, x) = image.at<Vec3b>(y, x + 1);
			}
		}
		found_seams--;
		width--;
		calculateEnergy();
		calculateCumulatedEnergy();
	}
}

void Image::findSeams(int n)
{
	std::vector<std::vector<bool>> used;
	used.resize(height);
	for (int y = 0; y < height; y++) 
	{
		used.at(y).resize(width);
	}

	paths.clear();
	paths.resize(n);

	found_seams = 0;

	for (int i = 0; i < n; i++) 
	{
		calculateCumulatedEnergy();

		int min_x = 0;
		while (min_x < width && used.at(height - 1).at(min_x)) {
			min_x++;
		}

		if (min_x >= width) {
			return;
		}

		for (int x = 1; x < width - 1; ++x) {
			if (!used.at(height - 1).at(x)) {
				if (cumulated_energy.at<int>(height - 1, x) < cumulated_energy.at<int>(height - 1, min_x)) {
					min_x = x;
				}
			}
		}

		int x = min_x;
		paths.at(i).push_back(x);
		used.at(height - 1).at(x) = true;

		for (int y = height - 2; y >= 0; --y)
		{
			int new_x = x;
			if (x > 0 && cumulated_energy.at<int>(y, x - 1) < cumulated_energy.at<int>(y, new_x))
			{
				new_x = x - 1;
			}
			if (x < width - 1 && cumulated_energy.at<int>(y, x + 1) < cumulated_energy.at<int>(y, new_x)) 
			{
				new_x = x + 1;
			}

			x = new_x;

			if (used.at(y).at(x)) {
				paths.resize(found_seams);
				return;
			}

			paths.at(i).push_back(x);
			used.at(y).at(x) = true;
			energy.at<unsigned char>(y, x) = 255;
		}

		found_seams++;
		std::reverse(paths.at(i).begin(), paths.at(i).end());
	}
}

void Image::addSeams(bool flag)
{
	if (!flag)
	{
		for (int y = 0; y < height; y++)
		{
			for (int n = 0; n < found_seams; n++)

			{
				image.at<Vec3b>(y, paths.at(n).at(y)) = { 255, 0, 0 };
			}
		}
	}
	else
	{
		copyMakeBorder(image, image, 0, 0, 0, found_seams, BORDER_CONSTANT);
		for (int y = 0; y < height; y++)
		{
			int count = 0;
			for (int x = width - 1; x >= 0; x--)
			{
				for (int n = 0; n < found_seams; n++)
				{
					if (x == paths.at(n).at(y))
					{
						count++;
						if (x>0) image.at<Vec3b>(y, x + found_seams - count) = mixColors(image.at<Vec3b>(y, x), image.at<Vec3b>(y, x - 1));
						else image.at<Vec3b>(y, x + found_seams - count) = image.at<Vec3b>(y, x);
						break;
					}
					else
						image.at<Vec3b>(y, x + found_seams - count) = image.at<Vec3b>(y, x);
				}					
			}
		}
		width += found_seams;
		energy.release();
		calculateEnergy();
	}
}

void Image::deleteVerticalSeams(int n, std::string window_name)
{
	for (int x = 0; x < n; x++)
	{
		findSeam();
		if (demo)
		{
			deleteSeam(0);
			showImage(window_name);
			cvWaitKey(1);
		}
		deleteSeam(1);
		resizeImage();
	}
}

void Image::deleteHorizontalSeams(int n, std::string window_name)
{
	rotateImage();
	for (int y = 0; y < n; y++)
	{
		findSeam();
		if (demo)
		{
			deleteSeam(0);
			rotateImage();
			showImage(window_name);
			cvWaitKey(1);
			rotateImage();
		}
		deleteSeam(1);
		resizeImage();
	}
	rotateImage();
}

void Image::showImage(std::string window_name)
{
	imshow(window_name, image);
	saveFile();
}

void Image::showEnergy(std::string window_name)
{
	imshow(window_name, energy);
}

void Image::showCumulatedEnergy(std::string window_name)
{
	calculateCumulatedEnergy();
	imshow(window_name, normaliseMat());
}

void Image::resizeImage()
{
	image = image(Rect(0, 0, width, height));
	energy.release();
	calculateEnergy();
}

void Image::rotateImage()
{
	transpose(image, image);
	transpose(energy, energy);
	transpose(cumulated_energy, cumulated_energy);
	int temp = height;
	height = width;
	width = temp;
	calculateCumulatedEnergy();
}

Mat Image::normaliseMat() 
{
	Mat tmp = energy.clone();
	// Find biggest value
	int e_max = 0;
	for (int y = 0; y < height; ++y) 
	{
		for (int x = 0; x < width; ++x) 
		{
			if (cumulated_energy.at<int>(y, x) > e_max) {
				e_max = cumulated_energy.at<int>(y, x);
			}
		}
	}

	// Scale values to [0,255]
	if (e_max > 0) {
		for (int y = 0; y < height; ++y) 
		{
			for (int x = 0; x < width; ++x) 
			{
				tmp.at<unsigned char>(y, x) = (unsigned char)((255.0 / (float)e_max)*(float)cumulated_energy.at<int>(y, x));
			}
		}
	}
	return tmp;
}

Vec3b Image::mixColors(Vec3b &a, Vec3b &b)
{
	Vec3b s;
	s[0] = (a[0] + b[0]) / 2;
	s[1] = (a[1] + b[1]) / 2;
	s[2] = (a[2] + b[2]) / 2;
	return s;
}

void Image::saveFile()
{
	imwrite("out.jpg", image);
}
