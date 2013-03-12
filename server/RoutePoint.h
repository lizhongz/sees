#ifndef _ROUTE_POINT_H_
#define _ROUTE_POINT_H_

#include <string>
#include "Coordinate.h"

class RoutePoint
{
public:
	string		name;  // Route point name
	Coordinate	coor;  // Point Coordinate

	RoutePoint();

	RoutePoint(string routeName, Coordinate coord);

	~RoutePoint();
};

#endif
