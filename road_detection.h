#ifndef __ROAD_DETECTION_H
#define __ROAD_DETECTION_H

#include "object_detection.h"

class RoadDetection:public ObjectDetection
{
	public:
	RoadDetection(cv::Mat* src, cv::Mat* des);
	RoadDetection(cv::Mat* src, cv::Mat* des, int rx, int ry);
	~RoadDetection();

	public:
	bool detect();
	void get_offsets(double* x, double* y);
	void config(double para1 = 0., double para2 = 0., double para3 = 0.);

	private:
	std::vector<cv::Vec2f> roadEdges;	
	std::vector<cv::Vec2f> lines; // lines detected in the photo
	int photoIndex;

	private:
	void init();
	bool find_edge();
	void draw_lines(std::vector<cv::Vec2f> & linesx);
};

#endif
