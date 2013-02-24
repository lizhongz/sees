#ifndef _ROUTES_MANAGER_H_
#define _ROUTES_MANAGER_H_

#include <string>
#include "Route.h"

using namespace std;

class RoutesManager
{
public:
	RoutesManager();
	~RoutesManager();

	// Get route by source position name and destination name
	int get_route(const string src, const string dest, Route &route);
};

#endif
