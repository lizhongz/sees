#include <cmath>
#include <unistd.h>
#include <iostream>
#include "NavigationEngine.h"
#include "GPS.h"
#include "RoutesManager.h"
#include "LatLongUtility.h"
#include "log.h"

using namespace std;

NavigationEngine::NavigationEngine():gps(GPS((char *)"/dev/ttyACM0"))
{
	// Initialize devices
	init_devs();
}

NavigationEngine::~NavigationEngine()
{

}

void NavigationEngine::run()
{
	m_stop = false;

	try
	{
		navigate();
	
	} catch(...) { /* ... */ }

	m_stop = true;
}

int NavigationEngine::navigate()
{	
	// Begin to navigate
	FILE_LOG(logINFO) << "Navig: Begin to navigate";	

	// Guide VIP to side walk
	double tmpDist;
	if(detect_x_route(tmpDist) == -1)
	{
		FILE_LOG(logINFO) << "Navig: No x route";	
	}
	else 
	{
		FILE_LOG(logINFO) << "Navig: Detected x route, distance: " 
			<< tmpDist;	
	}

	int subRtNum = route.pnts.size();
	for(sub_rt_inx = 1; sub_rt_inx < subRtNum; sub_rt_inx++)	
	{
		stop_point(); // If stop navigating

		// Calculate sub-route length
		sub_rt_dist = LatLongUtility::calc_p2p_dist(cur_pos, 
			route.pnts[sub_rt_inx].coor);	
		sub_rt_rm_dist = sub_rt_dist;

		FILE_LOG(logINFO) << "Navig: Sub-route index: " << sub_rt_inx;
		FILE_LOG(logINFO) << "Navig: Sub-route length: " << sub_rt_dist;

		while(sub_rt_rm_dist > DIST_2_RTPNT_TRSHD)
		{
			stop_point(); // If stop navigating

			// On the sub-route
				
			// Locating and calculate remaining distance
			if(gps.locate(cur_pos) != 0)
			{
				// Fail to locate
				FILE_LOG(logERROR) << "Navig: GPS locating error";
				return -1;		
			}
			sub_rt_rm_dist = LatLongUtility::calc_p2p_dist(
				cur_pos, route.pnts[sub_rt_inx].coor);	
			
			FILE_LOG(logINFO) << "Navig: VIP's Position: " 
				<< cur_pos.lat << ", " << cur_pos.lon;
			FILE_LOG(logINFO) << "Navig: Remaining distance: " 
				<< sub_rt_rm_dist;

			usleep(LOCATING_INTVL);
		}	
	
		// Arriving at a route point
		
		if(sub_rt_inx == (subRtNum - 1))
		{
			// Arriving at destination
			FILE_LOG(logINFO) << "Navig: Arriving at destination";
		}
		else
		{
			// Calculate the angle between two sub route
			double angle = LatLongUtility::calc_angle(
				route.pnts[sub_rt_inx - 1].coor, 
				route.pnts[sub_rt_inx].coor, 
				route.pnts[sub_rt_inx].coor, 
				route.pnts[sub_rt_inx + 1].coor);	

			// Turning or crossing the road
			
			FILE_LOG(logINFO) << "Navig: Arriving at Route point: " 
				<< route.pnts[sub_rt_inx].coor.lat 
				<< route.pnts[sub_rt_inx].coor.lon;
			FILE_LOG(logINFO) << "Navig: Angle: " << angle;

			// Detect y route
			if(detect_y_route() == -1)
			{
				FILE_LOG(logINFO) << "Navig: No y route";	
			}
			else 
			{
				FILE_LOG(logINFO) << "Navig: Detected y route";	
			}
		}
	}
	
	return 0;
}

int NavigationEngine::setup_route(string srcName, string destName)
{
	RoutesManager	rtMng;

	// Find a route
	if(rtMng.get_route(srcName, destName, route) != 0)
	{
		// Fail to find a route
		FILE_LOG(logINFO) << "Navig: Route does not exist";
		return -1;
	}

	// Get current position by using GPS 
	if(gps.locate(cur_pos) != 0)
	{
		// Fail to locate
		// FILE_LOG(logERROR) << "Navig: GPS locating error";
		// return -1;		
	}

	// Calculate the distance to the first sub-route
	double dist2rt = LatLongUtility::calc_p2l_dist(cur_pos, 
		route.pnts[0].coor, route.pnts[1].coor);

	FILE_LOG(logINFO) << "Navig: VIP's Position: " << cur_pos.lat 
		<< ", " << cur_pos.lon;
	FILE_LOG(logINFO) << "Navig: Distance to route: " << dist2rt;

	if(abs(dist2rt) > DIST_2_RT_TRSHD)
	{
		// VIP is not close to the route
		return -1;	
	}

	return 0;
}

int NavigationEngine::init_devs()
{
	// Initialize GPS
	//gps = GPS((char *)"/dev/ttyACM0");
}

void NavigationEngine::set_env_detection(EnvDetection *pEnvDet)	
{
	this->p_env_det = pEnvDet;
}

int NavigationEngine::detect_x_route(double &dist)
{
	int i = 0;
	while(i++ < 10)
	{
		if(p_env_det->road_detect_x(&dist))
		{
			return 0;
		}		
		usleep(1000000);
	}

	return -1;
}

int NavigationEngine::detect_y_route()
{
	double tmpDist;
	int i = 0;
	while(i++ < 10)
	{
		if(p_env_det->road_detect_y(&tmpDist))
		{
			return 0;
		}		
		usleep(1000000);
	}

	return -1;
}

