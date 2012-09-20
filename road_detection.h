#ifndef __ROAD_DETECTION_H
#define __ROAD_DETECTION_H

#include "object_detection.h"
#include "MSAC.h"

#define MAX_NUM_LINES	200
//#define MAX_DISTANCE	100000

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
	//std::vector<cv::Vec2f> roadEdges;	
	//std::vector<cv::Vec2f> lines; // lines detected in the photo
	std::vector<cv::Vec4i> lines;
	MSAC msac;
	vector<vector<cv::Point> > lineSegments;
	vector<cv::Point> aux;
	std::vector<std::vector<std::vector<cv::Point> > > lineSegmentsClusters;
	std::vector<cv::Mat> vps;			// vector of vps: vps[vpNum], with vpNum=0...numDetectedVps
	std::vector<int> numInliers;
	int numVps;
	int photoIndex;

	private:
	void init();
	bool find_edge();
	void draw_lines(std::vector<cv::Vec2f> & linesx);
	double findPos();
};


#endif
