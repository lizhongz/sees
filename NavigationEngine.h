#ifndef _NAVIGATION_ENGINE_H_
#define _NAVIGATION_ENGINE_H_

#include <string>
#include "Route.h"
#include "RoutesManager.h"
#include "Coordinate.h"
#include "GPS.h"
#include "Runnable.h"

using namespace std;

class NavigationEngine : public Runnable
{
private:
	GPS		gps;		// GPS receiver
	Route		route;		// Navigation route
	Coordinate 	cur_pos; 	// Current postion of VIP
	int		sub_rt_inx;	// Index of next route point in route 
	double		sub_rt_dist;	// Distance of current sub-route	
	double		sub_rt_rm_dist;	// Remaining Distance from current position to the end of sub_rt
	int 		run_times;	// Navigation times
	// Only the distance for VIP to the route 
	// is less than this value can this engine navigate
	static constexpr double DIST_2_RT_TRSHD = 10.0;
	// Distance threshold for testing if 
	// VIP arrives at a route point
	static constexpr double DIST_2_RTPNT_TRSHD = 4.0;
	// Locating interval (unit:microsecond)
	static constexpr double LOCATING_INTVL = 1000000;
public:
	NavigationEngine();
	
	~NavigationEngine();

	// Initialize all the devieces
	int init_devs();

	// Navigate by knowing source name destination name
	int navigate();

	// Setup route according to the source and destination name
	int setup_route(string srcName, string destName);

	// Inherited from Runnable class
	void run();
	
	// Get current position
	int get_cur_pos(Coordinate &coor);

	// Get route
	int get_route(Route &rte);
};

#endif

