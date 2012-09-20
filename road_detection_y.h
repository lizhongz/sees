#ifndef __ROAD_DETECTION_Y_H
#define __ROAD_DETECTION_Y_H

#include "object_detection.h"
#include "MSAC.h"

#define TOP 0
#define LEFT 1
#define BOTTOM 2
#define RIGHT 3

#define MAX_NUM_LINES	200
#define MAX_DISTANCE	10000

class RoadDetectionY:public ObjectDetection
{
	public:
	RoadDetectionY(cv::Mat* src, cv::Mat* des);
	RoadDetectionY(cv::Mat* src, cv::Mat* des, int rx, int ry);
	~RoadDetectionY();

	public:
	bool detect();
	void get_offsets(double* x, double* y);
	void config(double para1 = 0., double para2 = 0., double para3 = 0.);

	private:
	int photoIndex;
	std::vector<cv::Vec4i> lines;
	MSAC msac;
	vector<vector<cv::Point> > lineSegments;
	vector<cv::Point> aux;
	std::vector<std::vector<std::vector<cv::Point> > > lineSegmentsClusters;
	std::vector<cv::Mat> vps;			// vector of vps: vps[vpNum], with vpNum=0...numDetectedVps
	std::vector<int> numInliers;
	int numVps;

	private:
	void init();
	bool find_edge();
	void draw_lines(std::vector<cv::Vec4i> & linesx);
	void drawCS(cv::Mat &im, std::vector<std::vector<std::vector<cv::Point> > > &lineSegmentsClusters, std::vector<cv::Mat> &vps);
};


#endif
