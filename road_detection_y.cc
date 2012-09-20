#include "road_detection_y.h"


RoadDetectionY::RoadDetectionY(cv::Mat* src, cv::Mat* des):ObjectDetection(src, des), photoIndex(0)
{
	minVotes = 80;
	minLength = 80;
	lowThres = 30;
	highThres = 300;
	maxGap = 40;
	rhoStep = 1.;
	thetaStep = PI/180.;
	numVps = 1;
	init();
}

RoadDetectionY::RoadDetectionY(cv::Mat* src, cv::Mat* des, int rx, int ry):ObjectDetection(src, des, rx, ry), photoIndex(0)
{
	resoX = rx;
	resoY = ry;
	minVotes = 80;
	minLength = 60;
	lowThres = 30;
	highThres = 300;
	maxGap = 40;
	rhoStep = 1.;
	thetaStep = PI/180.;
	numVps = 2;
	init();
}

RoadDetectionY::~RoadDetectionY()
{

}

bool RoadDetectionY::detect()
{
	bool isFind = find_edge();

	char file[255] = {'\0'};
	msac.drawCS(*srcImg, lineSegmentsClusters, vps);

	if(!isFind)
	{
		printf("ENV_DETECT: No route found vertically\n");
		sprintf(file, "result/undetected_r_y_%d.jpeg", photoIndex++);
		cv::imwrite(file, *srcImg);
		return false;
	}
	printf("ENV_DETECT: Route found vertically\n"); 
	sprintf(file, "result/detected_r_y_%d.jpeg", photoIndex++);
	cv::imwrite(file, *srcImg);

	// Draw line segments according to their cluster
	//msac.drawCS(*srcImg, lineSegmentsClusters, vps);
	return true;
}

void RoadDetectionY::get_offsets(double* x, double* y)
{
	double yy= 0.0;
	unsigned int vpIndex = 0;

	for(unsigned int vpNum = 0; vpNum < vps.size(); vpNum++)
	{
		cv::Point2f vpt(vps[vpNum].at<float>(0,0), vps[vpNum].at<float>(1,0));
		if(vps[vpNum].at<float>(2,0) != 0)
		{
			// Paint vp if inside the image
			if(vpt.x >=0 && vpt.x < resoX && vpt.y >=0 && vpt.y < resoY)
			{
				vpIndex = vpNum;
				break;
			}
		}
	}
	cv::Point2f vp(vps[vpIndex].at<float>(0,0), vps[vpIndex].at<float>(1,0));

	bool left = false;
	bool right = false;
	double dx = 0., dy = 0, k = 0., b = 0.;
	for(unsigned int i = 0; i < lineSegmentsClusters[vpIndex].size(); i++)
	{
		cv::Point pt1 = lineSegmentsClusters[vpIndex][i][0];
		cv::Point pt2 = lineSegmentsClusters[vpIndex][i][1];
		dx = pt2.x - pt1.x;
		dy = pt2.y - pt1.y;
		if(dy/dx > 0)
		{
			left = true;
			//printf("left\n");	
		}
		if(dy/dx < 0)
		{
			right = true;
			//printf("right\n");	
		}
	}
	cv::Point point1;
	cv::Point point2;
	cv::Point point0(resoX/2, resoY);
	double A, B, C, dis = 0;
	
	if(left && right)
	{
		*x = 0;
		*y = 0;
		return;
	}
	else if(left)
	{
		unsigned int numLines = lineSegmentsClusters[vpIndex].size();
		point1 = lineSegmentsClusters[vpIndex][numLines/2][0];
		point2 = lineSegmentsClusters[vpIndex][numLines/2][1];
		point1.x += 50;
		point2.x += 50;
		//printf("left:point1 x = %f, y = %f\n", point1.x, point1.y);
		//printf("left:point2 x = %f, y = %f\n", point2.x, point2.y);
	}
	else if(right)
	{
		unsigned int numLines = lineSegmentsClusters[vpIndex].size();
		point1 = lineSegmentsClusters[vpIndex][numLines/2][0];
		point2 = lineSegmentsClusters[vpIndex][numLines/2][1];
		point1.x -= 50;
		point2.x -= 50;
		//printf("left:point1 x = %f, y = %f\n", point1.x, point1.y);
		//printf("left:point2 x = %f, y = %f\n", point2.x, point2.y);
	}

	A = (point1.y - point2.y);
	B = (point2.x - point1.x);
	C = point1.x * (point2.y - point1.y) - point1.y * (point2.x - point1.x);
	dis = abs(A*point0.x + B*point0.y + C);
	dis = dis/(sqrt(A*A + B*B));
	printf("ENV_DETECT: The distance is %f\n", 0);
	cv::line(*desImg, point1, point2, cv::Scalar(0, 0, 0));

	*x = dis;
	*y = 0;
}

void RoadDetectionY::config(double para1, double para2, double para3) {
	if(para1 != 0)
		numVps = para3;
	if(para2 != 0)
		minLength = para2;
	if(para3 != 0)
		minVotes = para1;
}


void RoadDetectionY::init()
{
	lines.clear();

	cv::Canny(*srcImg, *desImg, 180, 120, 3);
	
	int mode = MODE_NIETO;
	bool verbose = false;
	cv::Size procSize = cv::Size(resoX, resoY);
	msac.init(mode, procSize, verbose);
	cv::resize(*srcImg, *srcImg, procSize);		
}


bool RoadDetectionY::find_edge()
{
	bool isYV = false; // check if the vanishing point is on the horizontal direction
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
	for(int v=0; v<vps.size(); v++)
	{
		printf("VP %d (%.3f, %.3f, %.3f)", v, vps[v].at<float>(0,0), vps[v].at<float>(1,0), vps[v].at<float>(2,0));
		fflush(stdout);
		double vpNorm = cv::norm(vps[v]);
		if(fabs(vpNorm - 1) < 0.001)
		{
			printf("(INFINITE)");
			fflush(stdout);
		}
		printf("\n");
		if(fabs(vps[v].at<float>(0, 0) + resoX/2.) < 10*resoX)
			isYV = true;
	}		
	if(!isYV)
		return false;
	return true;
	
}

void RoadDetectionY::drawCS(cv::Mat &im, std::vector<std::vector<std::vector<cv::Point> > > &lineSegmentsClusters, std::vector<cv::Mat> &vps)
{
	std::vector<cv::Scalar> colors;
	colors.push_back(cv::Scalar(0,0,255)); // First is RED
	colors.push_back(cv::Scalar(0,255,0)); // Second is GREEN 
	colors.push_back(cv::Scalar(255,0,0)); // Third is BLUE

	// Paint vps
	for(unsigned int vpNum = 0; vpNum < vps.size(); vpNum++)
	{
		if(vps[vpNum].at<float>(2,0) != 0)
		{
			cv::Point2f vp(vps[vpNum].at<float>(0,0), vps[vpNum].at<float>(1,0));

			// Paint vp if inside the image
			if(vp.x >=0 && vp.x < im.cols && vp.y >=0 && vp.y <im.rows)
			{
				circle(im, vp, 4, colors[vpNum], 2);	
				circle(im, vp, 3, CV_RGB(0,0,0), -1);				
			}
		}
	}
	// Paint line segments
	for(unsigned int c = 0; c < lineSegmentsClusters.size(); c++)
	{
		printf("line segments of vp[%d]\n", c);
		for(unsigned int i = 0; i < lineSegmentsClusters[c].size(); i++)
		{
			cv::Point pt1 = lineSegmentsClusters[c][i][0];
			cv::Point pt2 = lineSegmentsClusters[c][i][1];

			cv::line(im, pt1, pt2, colors[c], 1);
		}
	}	
}

void RoadDetectionY::draw_lines(std::vector<cv::Vec4i> & linesx)
{
	std::vector<cv::Vec4i>::const_iterator it = linesx.begin();
	std::cout << "Drawing lines: " << linesx.size() << std::endl;
	int i = 0;
	while(it != linesx.end())
	{
		std::cout << "the line: " << i++ << std::endl;
		cv::Point p1((*it)[0], (*it)[1]);
		cv::Point p2((*it)[2], (*it)[3]);
		std::cout << "point1:" << (*it)[0] << " " << (*it)[1] << std::endl;
		std::cout << "point2:" << (*it)[2] << " " << (*it)[3] << std::endl;
		//if((*it)[0] <= 5 || (*it)[1] >= 640 || (*it)[2] <= 5 || (*it)[3] >= 640)
		cv::line(*srcImg, p1, p2, cv::Scalar(255, 255, 255));
		++it;
	}
}

