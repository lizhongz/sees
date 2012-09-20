#ifndef __OBJ_DETECT_H
#define __OBJ_DETECT_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <vector>
#include <cstdlib>

#ifndef PI
#define PI 3.1415926535897932
#endif

class ObjectDetection
{
	public:
	ObjectDetection(cv::Mat* src, cv::Mat* des);
	ObjectDetection(cv::Mat* src, cv::Mat* des, int rx, int ry);
	~ObjectDetection();
	
	public:
	virtual void config(double para1 = 0., double para2 = 0., double para3 = 0.) = 0;
	virtual bool detect() = 0;
	virtual void get_offsets(double* x, double* y) = 0;

	void set_threshold(int low, int high);
	void set_hough_para(double rho = 1., double theta = PI/180.);


	protected:
	virtual bool find_edge() = 0;
	double get_offset_y(double y);
	double get_offset_x(double x);
	void set_scale(double heightPx, double alphax, double betax);
	double compute_scale();


	public:
	cv::Mat* srcImg;
	cv::Mat* desImg;
	int minVotes;
	int minLength;
	int maxGap;	
	int lowThres;
	int highThres;
	double rhoStep;
	double thetaStep;
	double alpha; // the view angle of camera and the line intersecting with land 
	double beta; // the  vision angle of the camera
	int resoX; // width of the photo
	int resoY; // height of the photo
	double height; // the height of the camera to the land in photo scale
	double heightP; // the pratical height of the camera to the land, the unit is meters
};

#endif
