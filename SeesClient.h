#ifndef _SEES_CLIENT_H_
#define _SEES_CLIENT_H_

#include "CommProtocol.h"
#include "Client.h"
#include "Runnable.h"

class SeesClient : public Client, public Runnable
{
private:
	void *p_ctrl; // Point to Control class

	CommProtocol prot; // Communication protocol

public:
	SeesClient();

	~SeesClient();

	// Set pointer of Control class
	void set_p_ctrl(void *pCtrl); 

	// Communicate with sees server		
	int communicate();

	// Send route to server
	int send_route();

	// Send track point to server
	int send_track_point();

	// Send navigation finish command to server
	int send_finish_cmd();

	// Inherited from Runnable class
	void run();
};

#endif // _SEES_CLIENT_H_
