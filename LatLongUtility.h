// latitude and longitude related calculation

#ifndef _LAT_LONG_UTILITY_
#define _LAT_LONG_UTILITY_

#include "Coordinate.h"

class LatLongUtility
{
private:
	static const double RADIUS_EARTH = 6371000.0; // Radius of Earth (unit meter)

public:
	// Calculate two coordinates' distance (Rhumb line)
	static double calc_p2p_dist(const Coordinate &x, const Coordinate &y);

	// Convert latitude and longitude degree to radian
	static double deg2rad(double deg);
	
	// Convert radian to degree
	static double rad2deg(double rad);
	
	// Calculate Rhumb line's bearing in radian
	static double calc_bearing(const Coordinate &x, 
		const Coordinate &y);
	
	// Calculate a Rhumb line's direction in degree
	static double calc_direction(const Coordinate &x, 
		const Coordinate &y);

	// Calculate the distance of a point from a line
	static double calc_p2l_dist(const Coordinate &pnt, 
		const Coordinate &stPnt, const Coordinate &endPnt);

	// Calculate the anlge between two line in degree
	static double calc_angle(const Coordinate &stPntL1, 
		const Coordinate &endPntL1, const Coordinate &stPntL2, 
		const Coordinate &endPntL2);

	// Calculate destination by knowing the original 
	// cordinate, bearing and distance
	static void calc_destination(const Coordinate &x, 
		double brng, double dist, Coordinate &dest);
};

#endif


