
#include <iostream>
#include <fcntl.h>
#include "GPS.h"
#include "Coordinate.h"
#include "RoutesManager.h"
#include "LatLongUtility.h"
#include "NavigationEngine.h"

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
	NavigationEngine nvgEng;
	
	int i = 0;
	while(i++ < 4)
		nvgEng.navigate("ISIMA GATE", "TRAM Station");

	return 0;
}

