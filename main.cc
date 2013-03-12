
#include <iostream>
#include <fcntl.h>
#include "GPS.h"
#include "Coordinate.h"
#include "RoutesManager.h"
#include "LatLongUtility.h"
#include "NavigationEngine.h"
#include "log.h"
#include "unistd.h"
#include "Control.h"

using namespace std;

int main()
{
	Control ctrl;
	ctrl.schedule();

	return 0;
}

