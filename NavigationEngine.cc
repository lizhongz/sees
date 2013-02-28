#include <cmath>
#include <unistd.h>
#include <iostream>
#include "NavigationEngine.h"
#include "GPS.h"
#include "RoutesManager.h"
#include "LatLongUtility.h"

using namespace std;

NavigationEngine::NavigationEngine():gps(GPS((char *)"/dev/ttyACM0"))
{
	// Initialize devices
	init_devs();
}

NavigationEngine::~NavigationEngine()
{

}

int NavigationEngine::navigate(string srcName, string destName)
{	
	RoutesManager	rtMng;

	// Find a route
	if(rtMng.get_route(srcName, destName, route) != 0)
	{
		// Fail to find a route
		cout << "Error: findding route" << endl;
		return -1;
	}

	// Get current position by using GPS 
	if(gps.locate(cur_pos) != 0)
	{
		// Fail to locate
		cout << "Error: locating" << endl;
		return -1;		
	}

	// Calculate the distance to the first sub-route
	double dist2rt = LatLongUtility::calc_p2l_dist(cur_pos, 
		route.pnts[0].coor, route.pnts[1].coor);

	cout << "Current Position: " << cur_pos.lat << ", " << cur_pos.lon << endl;
	cout << "Distance to route: " << dist2rt << endl;

	if(abs(dist2rt) > DIST_2_RT_TRSHD)
	{
		// VIP is not close to the route
		cout << "VIP is not close to route" << endl;
		return -1;	
	}

	cout << "Begin to navigate" << endl;

	// Begin to navigate
	
	int subRtNum = route.pnts.size();
	for(sub_rt_inx = 1; sub_rt_inx < subRtNum; sub_rt_inx++)	
	{
		// Calculate sub-route length
		sub_rt_dist = LatLongUtility::calc_p2p_dist(cur_pos, 
			route.pnts[sub_rt_inx].coor);	
		sub_rt_rm_dist = sub_rt_dist;

		cout << "Route index: " << sub_rt_inx << endl;
		cout << "Route length: " << sub_rt_dist << endl;

		while(sub_rt_rm_dist > DIST_2_RTPNT_TRSHD)
		{
			// On the sub-route
				
			usleep(LOCATING_INTVL);

			// Locating and calculate remaining distance
			if(gps.locate(cur_pos) != 0)
			{
				// Fail to locate
				return -1;		
			}
			sub_rt_rm_dist = LatLongUtility::calc_p2p_dist(
				cur_pos, route.pnts[sub_rt_inx].coor);	
			
			cout << "Current Position: " << cur_pos.lat << ", " << cur_pos.lon << endl;
			cout << "Remain distance: " << sub_rt_rm_dist << endl;
		}	
	
		// Arriving at a route point
		
		if(sub_rt_inx == (subRtNum - 1))
		{
			// Arriving at destination
			cout << "Arriving at destination" << endl;

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
			
			cout << "Route point: " << route.pnts[sub_rt_inx].coor.lat 
				<< route.pnts[sub_rt_inx].coor.lon << endl;
			cout << "angle: " << angle << endl;
		}
	}
}

int NavigationEngine::init_devs()
{
	// Initialize GPS
	//gps = GPS((char *)"/dev/ttyACM0");
}
