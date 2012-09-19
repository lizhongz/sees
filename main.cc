
#include <iostream>
#include <fcntl.h>
#include "GPS.h"
#include "Coordinate.h"
#include "RoutesManager.h"
#include "LatLongUtility.h"
#include "NavigationEngine.h"
#include "log.h"

using namespace std;

int main()
{
	NavigationEngine nvgEng;
	
	FILE* pFile = fopen("app.log", "w");
	Output2FILE::Stream() = pFile;
        FILELog::ReportingLevel() = FILELog::FromString("DEBUG1");

#if 1 // test Navigation
	while(1)
	{
		if(nvgEng.navigate("ISIMA GATE", "TRAM Station") == 0)
			break;
	}
#endif

#if 0 // test GPS
	int i = 0;
	GPS gps((char *)"/dev/ttyACM0");
	Coordinate coor;
	while(1)
	{
		//nvgEng.navigate("ISIMA GATE", "TRAM Station");
		gps.locate(coor);
	}
#endif

	return 0;
}

