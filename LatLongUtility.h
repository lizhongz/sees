// latitude and longitude related calculation

#ifndef _LAT_LONG_UTILITY_
#define _LAT_LONG_UTILITY_

#include "Coordinate.h"

class LatLongUtility
{
private:
	static constexpr double RADIUS_EARTH = 6371000.0; // Radius of Earth (unit meter)

public:
	/**
  	 * Calculate two coordinates' distance (Rhumb line)
 	 * @param x a point's coordinate
 	 * @param y another point's coordinate
 	 * @return the distance between the two points (unit meter)
 	 */
	static double calc_p2p_dist(const Coordinate &x, const Coordinate &y);

	/** 
 	 * Convert degrees to radians
 	 * @param deg degrees
 	 * @return corresponding radians of the degrees
 	 */
	static double deg2rad(double deg);
	
	/** 
  	 * Convert radians to degrees
 	 * @param rad radians
 	 * @return corresponding degree of the radians
 	 */
	static double rad2deg(double rad);

	/** 
  	 * Calculate Rhumb line's bearing(unit: radian)
 	 * @param x the start coordinate
 	 * @param y the end coordinate
 	 * @return the bearing in the range [-pi, pi]
 	 */
	static double calc_bearing(const Coordinate &x, 
		const Coordinate &y);
	
	/** 
  	 * Calculate Rhumb line's direction (unit: radian)
 	 * @param x the start coordinate
 	 * @param y the end coordinate
 	 * @return the direction in the range [-180, 180 degree]
 	 */
	static double calc_direction(const Coordinate &x, 
		const Coordinate &y);

	/** 
  	 * Calculate the distance of a point from a line
 	 * @param pnt a point's coordinate
 	 * @param stPnt the start point's coordinate of the line
 	 * @param endPnt the end point's coordinate of the line
 	 * @return the distance of a point from a line (unit: meter)
 	 */
	static double calc_p2l_dist(const Coordinate &pnt, 
		const Coordinate &stPnt, const Coordinate &endPnt);

	/** 
  	 * Calculate the anlge between two line in degree
 	 * @param stPntL1 the start point's coordinate of line 1
 	 * @param endPntL1 the end point's coordinate of line 1
 	 * @param stPntL2 the start point's coordinate of line 2
 	 * @param endPntL2 the end point's coordinate of line 2
 	 * @return the angle degree in range [-180, 180]
 	 */
	static double calc_angle(const Coordinate &stPntL1, 
		const Coordinate &endPntL1, const Coordinate &stPntL2, 
		const Coordinate &endPntL2);

	/** 
  	 * Calculate destination by knowing the source point's
  	 * cordinate, bearing and distance
 	 * @param x the source point's coordinate
 	 * @param brng the bearing
 	 * @param dit the distance
 	 * @param dest the destination point's coordinate
 	 */
	static void calc_destination(const Coordinate &x, 
		double brng, double dist, Coordinate &dest);
};

#endif


