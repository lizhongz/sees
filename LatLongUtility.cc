#include <cmath>
#include "LatLongUtility.h"

double LatLongUtility::calc_distance(Coordinate x, Coordinate y)
{
	// Haversine formula
	
	double latXInRad = x.lat * (M_PI / 180.0);
	double lonXInRad = x.lon * (M_PI / 180.0);
	double latYInRad = y.lat * (M_PI / 180.0);
	double lonYInRad = y.lon * (M_PI / 180.0);

	double dLatInRad = latYInRad - latXInRad;
	double dLonInRad = lonYInRad - lonXInRad;

	double a = pow(sin(dLatInRad / 2.0), 2.0)
		+ cos(latXInRad) * cos(latYInRad) * pow(sin(dLonInRad / 2.0), 2.0);

	double c = 2.0 * atan2(sqrt(a), sqrt(1 - a));

	double distance =  RADIUS_EARTH * c;
	
	return distance;
}

double LatLongUtility::calc_cross_track_dist(Coordinate pnt, 
	Coordinate stPnt, Coordinate endPnt)
{

}

