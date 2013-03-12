#ifndef _ROUTE_H_
#define _ROUTE_H_

#include <vector>
#include "RoutePoint.h"

using namespace std;

class Route
{
public:
	vector <RoutePoint> pnts; // Route points
	string name; // Route name

	Route();
	~Route();
};

#endif
