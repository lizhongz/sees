#ifndef __ENV_DETECT_H
#define __ENV_DETECT_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <vector>
#include <cstdlib>

#include "camera.h"
#include "road_detection.h"
#include "road_detection_y.h"

#define VIDEO_DEV "/dev/video0"
#define FILE_NAME "sees_spot_"

class EnvDetection
{
	public:
	EnvDetection();
	~EnvDetection();
	
	public:
	bool road_detect_x(double *distance);
	bool road_detect_y(double *distance);

	private:
	CCamera * camera;
	RoadDetection* roadX;
	RoadDetectionY* roadY;
	cv::Mat srcImg;
	cv::Mat desImg;

	private:
	void take_photos();

};

#endif
