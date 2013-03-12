#ifndef _COMMPROTOCOL_H_
#define _COMMPROTOCOL_H_

#include <string>
#include "Route.h"
#include "pugiconfig.h"
#include "pugixml.h"

using namespace std;

class CommProtocol
{
public:
	CommProtocol();
	
	~CommProtocol();

	// Envelop a route packet(Head info 
	// and route info in XML format)
	int envelop_route(const Route &rte, string &pck);

	// Envelop a track point packet (Head info 
	// and track point info in XML format)
	int envelop_coord(const Coordinate &coord, string &pck);

	// Envelop a cancel command packet
	int envelop_cancel_cmd(string &pck);

	// Envelop a finish command packet
	int envelop_finish_cmd(string &pck);

	// Parse a SEES protocol packet
	int parse(const string &pkt, string &type, string &content);
private:
};

#endif // _COMMPROTOCOL_H_
