#include "env_detection.h"

EnvDetection::EnvDetection()
{
	camera = new CCamera((char*)VIDEO_DEV);
	roadX= NULL;
	roadY = NULL;
	camera->cam_init();
}

EnvDetection::~EnvDetection()
{
	camera->cam_quit();
	delete camera;
	delete roadX;
	delete roadY;

}

void EnvDetection::take_photos()
{
	camera->cam_save_photo(FILE_NAME, 4);
	char file[255] = {'\0'};
	sprintf(file, "%s_3.jpeg", FILE_NAME);
	srcImg = cv::imread(file, 0);
}
	
bool EnvDetection::road_detect_x(double *distance)
{
	take_photos();
	if(roadX == NULL)
		roadX = new RoadDetection(&srcImg, &desImg);
	if(!roadX->detect())
	{	
		*distance = 0.;
		return false;
	}
	double x = 0.;
	roadX->get_offsets(&x, distance);
	return true;
}

bool EnvDetection::road_detect_y(double *distance)
{
	take_photos();
	if(roadY == NULL)
		roadY = new RoadDetectionY(&srcImg, &desImg);
	if(!roadY->detect())
	{
		*distance = 0.;
		return false;
	}
	double y = 0.;
	roadY->get_offsets(distance, &y);
	return true;
}
