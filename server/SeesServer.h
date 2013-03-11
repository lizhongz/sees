#ifndef _SEES_SERVER_H_
#define _SEES_SERVER_H_

#include "Server.h"

class SeesServer : public Server
{
public:
	// Handle client's request
	int handle_clt_req(int cltfd, string req);

	// Handle navigation command
	int handle_nvigt_cmd(string data);
	
	// Handle position command
	int handle_pos_cmd(string data);
	
	// Handle cancel command
	int handle_cancel_cmd();

	// Handle finish command
	int handle_finish_cmd();
};

#endif //_SEES_SERVER_H_


