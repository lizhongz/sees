#include "SeesServer.h"
#include "Config.h"
#include "log.h"
#include "GPXHelper.h"

int main()
{
	SeesServer srv;

	if(srv.setup(SERVER_PORT) == -1)
	{
		FILE_LOG(logERROR) << "Net: Failed to setup server";
	}
	FILE_LOG(logDEBUG) << "Net: Setuped server";

	srv.wait_conn();	

	return 0;
}
