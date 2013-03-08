
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
#include "Client.h"
#include "Config.h"

using namespace std;

int main()
{
#if 0
	Client *pClt = new Client();

	if(pClt->connect2srv(SERVER_ADDR, SERVER_PORT) == -1)
	{
		FILE_LOG(logERROR) << "Net: Connection error";
	}

	string str;
	while(1)
	{
		cin >> str;
		if(str.compare("q") == 0)
			break;

		pClt->send_data(str);
	}
#endif
		
	Control ctrl;
	ctrl.schedule();

	return 0;
}

