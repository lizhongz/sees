#include <sstream>
#include "GPXHelper.h"
#include "log.h"

int GPXHelper::create_rte_node(const Route &rte, 
	xml_node &parentNode, xml_node &rteNode)
{
	// Add route node
	rteNode = parentNode.append_child("rte");

	FILE_LOG(logDEBUG1) << to_string(rteNode);
	
	// Add route name node
	xml_node descr = rteNode.append_child("name");
	descr.append_child(pugi::node_pcdata).set_value(rte.name.c_str());
	FILE_LOG(logDEBUG1) << to_string(rteNode);

	// Add route point nodes
	xml_node rtePntNode;
	for(int i = 0; i < rte.pnts.size(); i++)
	{
		create_rtept_node(rte.pnts[i], rteNode, rtePntNode);	
		FILE_LOG(logDEBUG1) << to_string(rteNode);
	}

	FILE_LOG(logDEBUG1) << to_string(rteNode);

	return 0;
}

int GPXHelper::create_rtept_node(const RoutePoint &rtept, 
	xml_node &parentNode, xml_node &rtePntNode)
{
	// Add route point node
	rtePntNode = parentNode.append_child("rtept");

	// Add longitude and latitude attribut 
	rtePntNode.append_attribute("lon") = rtept.coor.lon;
	rtePntNode.append_attribute("lat") = rtept.coor.lat;

	// Add Route point name
	xml_node descr = rtePntNode.append_child("name");
	descr.append_child(pugi::node_pcdata
		).set_value(rtept.name.c_str());
	
	return 0;	
}

int GPXHelper::create_trkpt_node(const Coordinate &coord, 
	xml_node &parentNode,xml_node &trkPntNode)
{
	// Add track point
	trkPntNode = parentNode.append_child("trkpt");

	// Add longitude and latitude attribut 
	trkPntNode.append_attribute("lon") = coord.lon;
	trkPntNode.append_attribute("lat") = coord.lat;
	
	// Add UTC time info
	xml_node descr = trkPntNode.append_child("time");
	descr.append_child(pugi::node_pcdata).set_value(coord.utc.c_str());

	return 0;
}

string GPXHelper::to_string(xml_node &node)
{
	ostringstream os;

	node.print(os, "", pugi::format_raw);
	//node.print(os, "\t");
	return os.str();
}
