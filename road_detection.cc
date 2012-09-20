#include "road_detection.h"


RoadDetection::RoadDetection(cv::Mat* src, cv::Mat* des):ObjectDetection(src, des), photoIndex(0)
{
	minVotes = 80;
	minVotes = 150;
	minLength = 100;
	lowThres = 125;
	highThres = 200;
	maxGap = 30;
	rhoStep = 1.;
	thetaStep = PI/180.;
	init();
}

RoadDetection::RoadDetection(cv::Mat* src, cv::Mat* des, int rx, int ry):ObjectDetection(src, des, rx, ry), photoIndex(0)
{
	minVotes = 80;
	minVotes = 150;
	minLength = 100;
	lowThres = 125;
	highThres = 300;
	maxGap = 30;
	rhoStep = 1.;
	thetaStep = PI/180.;
	init();
}

RoadDetection::~RoadDetection()
{

}

bool RoadDetection::detect()
{
	bool isFind = find_edge();

	char file[255] = {'\0'};

	msac.drawCS(*srcImg, lineSegmentsClusters, vps);

	if(!isFind)
	{
		printf("ENV_DETECT: No route found horizontally\n");
		sprintf(file, "result/undetected_r_x_%d.jpeg", photoIndex++);
		cv::imwrite(file, *srcImg);
		return false;
	}
	printf("ENV_DETECT: Route found horizontally\n"); 
	sprintf(file, "result/detected_r_x_%d.jpeg", photoIndex++);
	cv::imwrite(file, *srcImg);
	
	return true;
}

double RoadDetection::findPos()
{
	double distance = 9999.;
	// get the distance from the nearest line
	for(unsigned int c = 0; c < lineSegmentsClusters.size(); c++)
	{
		//printf("line segments of vp[%d]\n", c);
		for(unsigned int i = 0; i < lineSegmentsClusters[c].size(); i++)
		{
			cv::Point pt1 = lineSegmentsClusters[c][i][0];
			cv::Point pt2 = lineSegmentsClusters[c][i][1];
			if(distance > (resoY - pt1.y))
				distance = resoY - pt1.y;
			if(distance > (resoY - pt2.y))
				distance = resoY - pt2.y;
		}
	}	
	return distance;
}

void RoadDetection::get_offsets(double* x, double* y)
{
	double yy= 0.0;
	yy = findPos();
	*x = 0;
	*y = get_offset_y(yy); 
}

void RoadDetection::config(double para1, double para2, double para3)
{
	if(para1 != 0.)
	{
		minVotes = para1;
	}
	init();
}


void RoadDetection::init()
{
	lines.clear();
	//cv::Canny(*srcImg, *desImg, lowThres, highThres);
	//cv::threshold(*desImg, *desImg, 128, 255, cv::THRESH_BINARY);
	//cv::HoughLines(*desImg, lines, rhoStep, thetaStep, minVotes);
	cv::Canny(*srcImg, *desImg, 180, 120, 3);
	
	int mode = MODE_NIETO;
	bool verbose = false;
	cv::Size procSize = cv::Size(resoX, resoY);
	msac.init(mode, procSize, verbose);
	cv::resize(*srcImg, *srcImg, procSize);		
}

bool RoadDetection::find_edge()
{
	bool isXV = false; // check if the vanishing point is on the horizontal direction
	int houghThreshold = 70;
	if(srcImg->cols * srcImg->rows < 400*400)
		houghThreshold = 100;		
	
	cv::HoughLinesP(*desImg, lines, 1, CV_PI/180, houghThreshold, 10, 10);

	while(lines.size() > MAX_NUM_LINES)
	{
		lines.clear();
		houghThreshold += 10;
		cv::HoughLinesP(*desImg, lines, 1, CV_PI/180, houghThreshold, 10, 10);
	}
	for(size_t i=0; i<lines.size(); i++)
	{		
		cv::Point pt1, pt2;
		pt1.x = lines[i][0];
		pt1.y = lines[i][1];
		pt2.x = lines[i][2];
		pt2.y = lines[i][3];
		line(*desImg, pt1, pt2, CV_RGB(0,0,0), 2);
		/*circle(outputImg, pt1, 2, CV_RGB(255,255,255), CV_FILLED);
		circle(outputImg, pt1, 3, CV_RGB(0,0,0),1);
		circle(outputImg, pt2, 2, CV_RGB(255,255,255), CV_FILLED);
		circle(outputImg, pt2, 3, CV_RGB(0,0,0),1);*/

		// Store into vector of pairs of Points for msac
		aux.clear();
		aux.push_back(pt1);
		aux.push_back(pt2);
		lineSegments.push_back(aux);
	}
	// Multiple vanishing points
	std::vector<std::vector<int> > CS;	// index of Consensus Set for all vps: CS[vpNum] is a vector containing indexes of lineSegments belonging to Consensus Set of vp numVp

	
	// Call msac function for multiple vanishing point estimation
	msac.multipleVPEstimation(lineSegments, lineSegmentsClusters, numInliers, vps, numVps); 
	for(unsigned int v=0; v<vps.size(); v++)
	{
		printf("ENV_DETECT: VP %d (%.3f, %.3f, %.3f)", v, vps[v].at<float>(0,0), vps[v].at<float>(1,0), vps[v].at<float>(2,0));
		fflush(stdout);
		double vpNorm = cv::norm(vps[v]);
		if(fabs(vpNorm - 1) < 0.001)
		{
			printf("(INFINITE)");
			fflush(stdout);
		}
		printf("\n");
		if(fabs(vps[v].at<float>(0,0) + resoX/2.) > 10*resoX)
			isXV = true;
	}		
	if(!isXV)
		return false;
	return true;
}

void RoadDetection::draw_lines(std::vector<cv::Vec2f> & linesx)
{
	std::vector<cv::Vec2f>::const_iterator it = linesx.begin();
	std::cout << "Drawing lines: " << linesx.size() << std::endl;
	while(it != linesx.end())
	{
		float rho = (*it)[0];
		float tt = (*it)[1];

		//if(tt < PI/2 + PI/180*1 && tt > PI/2 - PI/180*1)
		//{
		//	cv::Point pt1(0, rho/sin(tt));
		//	cv::Point pt2(src.cols, (rho-src.cols*cos(tt))/sin(tt));
		//	cv::line(src, pt1, pt2, cv::Scalar(255), 1);
		//}
		cv::Point pt1(0, rho/sin(tt));
		cv::Point pt2(srcImg->cols, (rho-srcImg->cols*cos(tt))/sin(tt));
		cv::line(*srcImg, pt1, pt2, cv::Scalar(0, 0, 0), 1);
		++it;
	}
}

