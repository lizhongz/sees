#ifndef _NAVIGATION_ENGINE_H_
#define _NAVIGATION_ENGINE_H_

#include <string>
#include "Route.h"
#include "RoutesManager.h"
#include "Coordinate.h"

using namespace std;

class NavigationEngine
{
private:
	Route		route;		// Navigation route
	Coordinate 	cur_pos; 	// Current postion of VIP
	int		rt_pnt_inx;	// Index of next route point in route 
	double		dist;		// Distance of current sub-route	
	double		distToNxPnt;	// Distance from current position to next route point 
	// Only the distance for VIP to the route 
	// is less than this value can this engine navigate
	static const double DIST_TO_ROUTE_TRHESHOLD = 10;
public:
	NavigationEngine();
	
	~NavigationEngine();

	// Navigate by knowing source name destination name
	int navigate(string srcName, string destName);
};

#endif

