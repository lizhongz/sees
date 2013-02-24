#include "RoutesManager.h"
#include "Coordinate.h"
#include "RoutePoint.h"
#include "Route.h"

RoutesManager::RoutesManager()
{

}

RoutesManager::~RoutesManager()
{

}

// Get route by source position name and destination name
int RoutesManager::get_route(const string src, const string dest, Route &route)
{
	Coordinate coor;
	RoutePoint rtPnt;

	coor.lat = 45.759291;
	coor.lon = 3.111344;
	rtPnt.name = "ISIMA GATE";
	rtPnt.coor = coor;	
	route.pnts.push_back(rtPnt);

	coor.lat = 45.758259;
	coor.lon = 3.112073;
	rtPnt.name = "";
	rtPnt.coor = coor;	
	route.pnts.push_back(rtPnt);

	coor.lat = 45.758843;
	coor.lon = 3.113892;
	rtPnt.name = "Tram Station";
	rtPnt.coor = coor;	
	route.pnts.push_back(rtPnt);

	return 0;
}
