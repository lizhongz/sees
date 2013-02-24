#include "NavigationEngine.h"
#include "GPS.h"
#include "RoutesManager.h"

NavigationEngine::NavigationEngine()
{

}

NavigationEngine::~NavigationEngine()
{

}

int NavigationEngine::navigate(string srcName, string destName)
{	
	GPS		gps;
	RoutesManager	rtMng;

	// Find a route
	if(rtMng.get_route(srcName, destName, this->route) != 0)
	{
		// Fail to find a route
		return -1;
	}

	// Get current position by using GPS 
	if(gps.positioning(this->cur_pos) != 0)
	{
		// Fail to locate
		return -1;		
	}

	// If VIP's position is close to the starting route point		
	double disToRoute = LatLongUtility::calc_distance(cur_pos, 
		route.pnts[0].coor);
	if(disToRoute)
			
	
}
