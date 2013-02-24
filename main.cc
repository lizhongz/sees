
#include <iostream>
#include <fcntl.h>
#include "GPS.h"
#include "Coordinate.h"
#include "RoutesManager.h"
#include "LatLongUtility.h"

using namespace std;

int main()
{
	Coordinate coor;
#if 0
	GPS gpsDev = GPS((char *)"/dev/ttyACM0");

	for(int i = 0; i < 1; i++)
	{
		gpsDev.positioning(coor);
		
		cout <<  coor.utc << ", ";
		cout <<  coor.lat << ", "; 
		cout <<  coor.lon << endl;
	}

#endif
	RoutesManager rtsMng;
	Route rt;

	rtsMng.get_route("ISIMA GATE", "TRAM Station", rt);
	
	double distance;
	distance = LatLongUtility::calc_distance(rt.pnts[0].coor, rt.pnts[1].coor);
	cout << distance << endl;

	return 0;
}

