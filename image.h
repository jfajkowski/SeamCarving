#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>

using namespace cv;
extern bool demo;
extern int edge_type;

class Image
{
public:
	Image(){};
	Image(const Mat &src, string n);
	~Image(){};
	int returnWidth() { return width; }
	int returnHeight(){ return height; }

	void calculateEnergy();
	void calculateCumulatedEnergy();

	void calculatePixelEnergy(int x, int y);
	void findSeam();
	void deleteSeam(bool flag);

	void findSeams(int n);
	void addSeams(bool flag);

	void deleteVerticalSeams(int n, string window_name);
	void deleteHorizontalSeams(int n, string window_name);

	//Show images
	void showImage(string window_name);
	void showEnergy(string window_name);
	void showCumulatedEnergy(string window_name);
	void resizeImage();
	void rotateImage();
	Mat normaliseMat();
	Vec3b mixColors(Vec3b &a, Vec3b &b);
	void saveFile();

private:
	int found_seams;
	string name;
	int height;
	int width;
	Mat_<Vec3b> image;
	Mat_<Vec3b> duplicate;
	Mat energy;
	Mat cumulated_energy;
	std::vector<std::vector<int>> paths;

};
