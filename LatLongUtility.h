// latitude and longitude related calculation

#ifndef _LAT_LONG_UTILITY_
#define _LAT_LONG_UTILITY_

#include "Coordinate.h"

class LatLongUtility
{
private:
	static const double RADIUS_EARTH = 6371000.0; // Radius of Earth (unit m)

public:
	// Calculate two coordinates' distance
	static double calc_distance(Coordinate x, Coordinate y);
};

#endif


