
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
/*
	double distance = LatLongUtility::calc_p2p_dist(rt.pnts[0].coor, rt.pnts[1].coor);
	cout << distance << endl;
	distance = LatLongUtility::calc_p2p_dist(rt.pnts[1].coor, rt.pnts[2].coor);
	cout << distance << endl;

	double direction = LatLongUtility::calc_direction(rt.pnts[0].coor, rt.pnts[1].coor);
	cout << direction << endl;
	direction = LatLongUtility::calc_direction(rt.pnts[1].coor, rt.pnts[2].coor);
	cout << direction << endl;
	double angle = LatLongUtility::calc_angle(rt.pnts[0].coor, rt.pnts[1].coor, rt.pnts[1].coor, rt.pnts[2].coor);
	cout << angle << endl;

	Coordinate coor1s("sdf", 0, 0);
	Coordinate coor1e("abc", 0, -90);
	Coordinate coor2s("sdf", 0, 0);
	Coordinate coor2e("abc", 45, 0);
	angle = LatLongUtility::calc_angle(coor1s, coor1e, coor2s, coor2e);
	cout << angle << endl;
	
	Coordinate coor1s("sdf", 0, 0);
	Coordinate coor1e("abc", 0, -90);
	direction = LatLongUtility::calc_direction(coor1s, coor1e);
	cout << direction << endl;
	
	Coordinate coor2s("sdf", 0, 0);
	Coordinate coor2e("abc", 45, 0);
	direction = LatLongUtility::calc_direction(coor2s, coor2e);
	cout << direction << endl;

	Coordinate coor3s("sdf", 35, 45);
	Coordinate coor3e("abc", 35, 135);
	direction = LatLongUtility::calc_direction(coor3s, coor3e);
	cout << direction << endl;

	Coordinate coor4("sdf", 45.759323, 3.111432);
	double distance2 = LatLongUtility::calc_cross_track_dist(coor4, rt.pnts[0].coor, rt.pnts[1].coor);
	cout << distance2 << endl;
		
	Coordinate coor5("sdf", 45.75928, 3.111218);
	double distance3 = LatLongUtility::calc_cross_track_dist(coor5, rt.pnts[0].coor, rt.pnts[1].coor);
	cout << distance3 << endl;

	Coordinate coor6s("abc", 45.759303, 3.111338);
	Coordinate coor6e("sdd", 45.758822, 3.111631);
	//Coordinate coor6p("sdd", 45.759331, 3.111438);
	Coordinate coor6p("sdd", 45.759269, 3.11121);
	double dist6 = LatLongUtility::calc_p2l_dist(coor6p, coor6s, coor6e);
	cout << dist6 << endl;
*/
	double brng7 = LatLongUtility::calc_bearing(rt.pnts[0].coor, rt.pnts[1].coor);
	double distance7 = LatLongUtility::calc_p2p_dist(rt.pnts[0].coor, rt.pnts[1].coor);
	Coordinate dest7;
	LatLongUtility::calc_destination(rt.pnts[0].coor, brng7, distance7, dest7);
	cout << dest7.lat << ", " << dest7.lon << endl;

	
	return 0;
}

