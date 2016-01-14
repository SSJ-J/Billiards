#include <cv.h>
#include <highgui.h>

#include "noise.h"

#include <vector>
#include <iostream>

using namespace std;

int main()
{
	IplImage *src = cvCreateImage(cvSize(256, 256), IPL_DEPTH_32S , 3);

	PerlinNoiseCreator p;
	vector<int> v = p.generateNoise(256, 256);
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			unsigned int temp = v[i * 256 + j];
			int r = (temp & 0x00ff0000) >> 16;
			int g = (temp & 0x0000ff00) >> 8;
			int b = (temp & 0x000000ff);
			cvSet2D(src, i, j, CV_RGB(b, g, r));
		}
	}
	cvSaveImage("3.bmp", src);
	return 0;
}