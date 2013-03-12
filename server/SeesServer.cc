#include <string>
#include <iostream>
#include "SeesServer.h"
#include "CommProtocol.h"
#include "GPXHelper.h"
#include "pugiconfig.h"
#include "pugixml.h"
#include "Config.h"

using namespace std;
using namespace pugi;

int SeesServer::handle_clt_req(int cltfd, string req)
{
	CommProtocol prot;
	string type;
	string content;

	//cout << "Received: " << req << endl;
	
	prot.parse(req, type, content);
	cout << type << ", " << content << endl;	

	if(type.compare(CMD_NVIGT) == 0)
	{
		handle_nvigt_cmd(content);			
	}
	else if(type.compare(CMD_POSITION) == 0) 
	{ 
		handle_pos_cmd(content);
	}
	else if(type.compare(CMD_CANCEL) == 0)
	{
		handle_cancel_cmd();
	}
	else if(type.compare(CMD_FINISH) == 0)
	{
		handle_finish_cmd();
	}
	else
	{
		// Wrong command type
		return -1;
	}

	return 0;
}

int SeesServer::handle_nvigt_cmd(string data)
{
	xml_document doc;
	GPXHelper gpxHlpr;
	
	// Load route to xml document object
	xml_parse_result result = doc.load_buffer(
		data.c_str(), data.size());
	if(!result)
	{
		// Parsed with errors
		return -1;	
	}
		
	// Extract route node
	xml_node rteNode = doc.child("rte");
	cout << gpxHlpr.to_string(rteNode) << endl;	
	
	// Save route to gpx file
	gpxHlpr.save_route(GPX_PATH, GPX_ROUTE_NAME, rteNode);	

	return 0;
}

int SeesServer::handle_pos_cmd(string data)
{
	xml_document doc;
	GPXHelper gpxHlpr;
	
	// Load route to xml document object
	xml_parse_result result = doc.load_buffer(
		data.c_str(), data.size());
	if(!result)
	{
		// Parsed with errors
		return -1;	
	}
		
	// Extract track point node
	xml_node trkptNode = doc.child("trkpt");

	// Save track point to gpx file
	gpxHlpr.save_track_point(GPX_PATH, GPX_TRACK_NAME, trkptNode);	
}
int SeesServer::handle_cancel_cmd()
{
	GPXHelper gpxHlpr;

	// Remove the route node of route gpx file
	gpxHlpr.remove_route(GPX_PATH, GPX_ROUTE_NAME);	
}

int SeesServer::handle_finish_cmd()
{
	GPXHelper gpxHlpr;

	// Remove the route node of route gpx file
	gpxHlpr.remove_route(GPX_PATH, GPX_ROUTE_NAME);	
}
