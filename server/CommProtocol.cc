#include <sstream>
#include <string>
#include "CommProtocol.h"
#include "Config.h"
#include "GPXHelper.h"
#include "log.h"

using namespace std;
using namespace pugi;

CommProtocol::CommProtocol()
{

}
	
CommProtocol::~CommProtocol()
{

}

int CommProtocol::envelop_route(const Route &rte, string &pck)
{
	GPXHelper gpxHlper;
	xml_node rteNode;

	// Create route node
	gpxHlper.create_rte_node(rte, rteNode);

	// Save XML node object to a string
	// and add head info for the packet
	pck = PROT_NAME;
	pck.append(CMD_NVIGT);
	pck.append(gpxHlper.to_string(rteNode));	

	return 0;
}

int CommProtocol::envelop_coord(const Coordinate &coord, string &pck)
{
	GPXHelper gpxHlper;
	xml_node trkPntNode;

	// Create track point node
	gpxHlper.create_trkpt_node(coord, trkPntNode);

	// Save XML node object to a string
	// and add head info for the packet
	pck = PROT_NAME;
	pck.append(CMD_POSITION);
	pck.append(gpxHlper.to_string(trkPntNode));	

	return 0;
}

int CommProtocol::envelop_cancel_cmd(string &pck)
{
	// Add head info for cancel command
	pck = PROT_NAME;
	pck.append(CMD_CANCEL);

	return 0;
}

int CommProtocol:: envelop_finish_cmd(string &pck)
{
	// Add head info for finish command
	pck = PROT_NAME;
	pck.append(CMD_FINISH);

	return 0;
}

int CommProtocol::parse(const string &pkt, string &type, string &content)
{
	// Extract protocol name
	string name = pkt.substr(0, PROT_NAME_SIZE);
	if(name.compare(PROT_NAME) != 0)
	{
		// Not a SEES protocol packet
		FILE_LOG(logINFO) << "Protocol: Parsed a wrong packet";
		return -1;
	}

	// Extract type info
	type = pkt.substr(PROT_NAME_SIZE, CMD_SIZE);

	// Extract content
	content = pkt.substr(PROT_HEAD_SIZE);
}

