#include "object_detection.h" 


ObjectDetection::ObjectDetection(cv::Mat* src, cv::Mat* des):srcImg(NULL), desImg(NULL), lowThres(0), highThres(0), rhoStep(0.), thetaStep(0.)
{
	resoX = 640;
	resoY = 480;
	if(src != NULL && des != NULL)
	{
		srcImg = src;
		desImg = des;
	}
	heightP= 1.0;
	alpha = PI/6.;
	beta = PI/2.;
}

ObjectDetection::ObjectDetection(cv::Mat* src, cv::Mat* des, int rx, int ry):srcImg(NULL), desImg(NULL), lowThres(0), highThres(0), rhoStep(0.), thetaStep(0.)
{
	resoX = rx;
	resoY = ry;
	if(src != NULL && des != NULL)
	{
		srcImg = src;
		desImg = des;
	}
	heightP= 1.0;
	alpha = PI/4.;
	beta = PI/3.;
}

ObjectDetection::~ObjectDetection()
{

}

void ObjectDetection::set_threshold(int low, int high)
{
	if(low != 0)
		lowThres = low;
	if(high != 0)
		highThres = high;
}

void ObjectDetection::set_hough_para(double rho, double theta)
{
	if(rho != 0. && theta != 0.)
	{
		rhoStep= rho;
		thetaStep = theta;
	}
}

double ObjectDetection::get_offset_y(double y)
{
	double scale = compute_scale();
	double hx = resoX/2./tan(beta/2.);
	double distance = y*hx/abs(height - y);
	return distance * scale;
}

double ObjectDetection::get_offset_x(double x)
{
	double scale = compute_scale();
	double hx = resoY/2./tan(beta/2.);
	double distance = x*hx/abs(height - x);
	return distance * scale;
}

void ObjectDetection::set_scale(double heightPx, double alphax, double betax)
{
	if(heightPx != 0)
		heightP = heightPx;
	if(alphax != 0)
		alpha = alphax;
	if(betax != 0)
		beta = betax;
}

double ObjectDetection::compute_scale()
{
	
	double hx = resoX/2./tan(beta/2.);
	height = hx/tan(alpha);
	double scale = heightP/height;
	return scale;
}
