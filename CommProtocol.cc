#include <sstream>
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
	GPXHelper	gpxHlper;
	xml_document	doc;
	xml_node	rteNode;

	// Create route node
	gpxHlper.create_rte_node(rte, doc, rteNode);

	string rteInfo = gpxHlper.to_string(rteNode);

	// Save XML node object to a string
	// and add head info for the packet
	pck = PROT_NAME;
	pck.append(CMD_NVIGT);
	pck.append(rteInfo);	

	return 0;
}

int CommProtocol::envelop_coord(const Coordinate &coord, string &pck)
{
	GPXHelper	gpxHlper;
	xml_document	doc;
	xml_node	trkPntNode;

	// Create track point node
	gpxHlper.create_trkpt_node(coord, doc, trkPntNode);

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

string CommProtocol::to_string(xml_node &node)
{
	ostringstream os;

	node.print(os, "", pugi::format_raw);
	//node.print(os, "\t");
	return os.str();
}
