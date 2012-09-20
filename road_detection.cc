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
	sprintf(file, "result/r_x_%d.jpeg", photoIndex++);
	cv::imwrite(file, *srcImg);
	if(isFind)
	{
		draw_lines(roadEdges);
		return true;
	}
	return false;
}
void RoadDetection::get_offsets(double* x, double* y)
{
	double yy= 0.0;

	// fetch the edges of the road, and store the nearest edge in rho
	if(roadEdges.empty())
	{
		*x = 0;
		*y = 0;
		return;
	}

	std::vector<cv::Vec2f>::const_iterator it = roadEdges.begin();
	double rho = 0.;
	double tt = 0.;
	while(it != roadEdges.end())
	{
		if(rho < (*it)[0])
		{
			rho = (*it)[0];
			tt = (*it)[1];
		}
		++it;
	}

	if(tt != 0)
		yy = (rho - resoX/2*cos(tt))/sin(tt);
	else
		yy = rho;

	yy = resoY - yy;

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
	cv::Canny(*srcImg, *desImg, lowThres, highThres);
	cv::threshold(*desImg, *desImg, 128, 255, cv::THRESH_BINARY);
	cv::HoughLines(*desImg, lines, rhoStep, thetaStep, minVotes);
}

bool RoadDetection::find_edge()
{
	bool isFind = false;
	//lines.clear();
	roadEdges.clear();
	std::vector<cv::Vec2f>::const_iterator it = lines.begin();
	std::cout << "the number of parallel lines is: " << lines.size() << std::endl;
	while(it != lines.end())
	{
		float rho = (*it)[0];
		float tt = (*it)[1];
		float rhox = 0.;
		float ttx = 0.;

		//std::cout << "the rho is: " << rho << std::endl;
		if(rho < 400)
		{
			++it;
			continue;
		}
		
		std::vector<cv::Vec2f>::const_iterator itx = it + 3;//lines.end() - 1;
		while(itx != lines.end())
		{
			rhox = (*itx)[0];
			ttx = (*itx)[1];
			//find a line whose ttx is similar to tt, which means a pair of parallel line	
			if(ttx < (tt + (PI/180)*5.5) && ttx > (tt - (PI/180)*5.5))	
				break;	
			++itx;
		}

		if(itx == it)
		{
			++it;
			continue;
		}
		
		//if(abs(rho - rhox) < 180)
		//{
		//	++it;
		//	continue;
		//}
		std::cout << "first line: rho = " << rho << "tt = " << tt << std::endl;
		std::cout << "second line: rho = " << rhox << "tt = " << ttx << std::endl;
		roadEdges.push_back(*it);
		roadEdges.push_back(*itx);
		isFind = true;
		break;
	}
	return isFind;
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

