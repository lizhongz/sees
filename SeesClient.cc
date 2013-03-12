#include <chrono>
#include <string>
#include <thread>
#include "SeesClient.h"
#include "log.h"
#include "Config.h"
#include "Coordinate.h"
#include "GPXHelper.h"
#include "Control.h"

using namespace std;

SeesClient::SeesClient() : p_ctrl(NULL)
{

}

SeesClient::~SeesClient()
{

}

void SeesClient::run()
{
	m_stop = false;
	
	try
	{
		communicate();			
	
	} catch(int i) { /* ... */ }

	m_stop = true;
}

int SeesClient::communicate()
{
	std::chrono::milliseconds dura(SEND_TIME_INTVL);	
	Coordinate	coor;
	string		pck;
	bool		preEngState = false;

	// Connect to server
	if(connect2srv(SERVER_ADDR, SERVER_PORT) == -1)
	{
		FILE_LOG(logERROR) << "Net: Connection error";
		return -1;
	}

	while(1)
	{
		stop_point();

		if(((Control*)p_ctrl)->get_nvigt_eng() == NULL)	
		{
			// If navigation engine is not opened
			FILE_LOG(logINFO) << "Client: Engine is not opened";
			std::this_thread::sleep_for(dura);
			continue;	
		}
	
		// If navigation engine's running state changed, client need to 
		// send finish cmd or route to sees server
		if(preEngState != ((Control*)p_ctrl)->get_nvigt_eng()->is_running())
		{
			if(preEngState == true)
			{
				// Navigation engine was stoped
				
				preEngState = false;
				send_finish_cmd();
			}
			else
			{
				// Navigation engine is running
				preEngState = true;
				send_route();
			}
		}
		
		// Send track point
		send_track_point();

		// Sleep for a while
		std::this_thread::sleep_for(dura);
	}	
}

int SeesClient::send_track_point()
{
	Coordinate	coor;
	string		pck;

	// Get current positon
	((Control*)p_ctrl)->get_nvigt_eng()->get_cur_pos(coor);
	// Envelop coordinate
	prot.envelop_coord(coor, pck);

	FILE_LOG(logDEBUG1) << "Client: Send " << pck;

	// Send track packet
	return send_data(pck);
}

int SeesClient::send_route()
{
	Route	rte;
	string	pck;

	// Get route	
	((Control*)p_ctrl)->get_nvigt_eng()->get_route(rte);
	// Envelop route
	prot.envelop_route(rte, pck);

	FILE_LOG(logDEBUG1) << "Client: Send " << pck;

	// Send track packet
	return send_data(pck);
}

int SeesClient::send_finish_cmd()
{
	string	pck;

	// Envelop finish command
	prot.envelop_finish_cmd(pck);

	FILE_LOG(logDEBUG1) << "Client: Send " << pck;

	// Send track packet
	return send_data(pck);
}

void SeesClient::set_p_ctrl(void *pCtrl)
{
	this->p_ctrl = pCtrl;
}

