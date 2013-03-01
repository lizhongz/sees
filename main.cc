
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

	int i = 0;
	while(1)
	{
		nvgEng.navigate("ISIMA GATE", "TRAM Station");
	}

	return 0;
}

