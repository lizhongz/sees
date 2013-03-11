#include <sstream>
#include "GPXHelper.h"
#include "log.h"
#include "Config.h"

int GPXHelper::create_rte_node(const Route &rte, xml_node &rteNode)
{
	xml_document doc;

	// Add route node
	rteNode = doc.append_child("rte");
	
	// Add route name node
	xml_node descr = rteNode.append_child("name");
	descr.append_child(pugi::node_pcdata).set_value(rte.name.c_str());

	// Add route point nodes
	xml_node rtePntNode;
	for(int i = 0; i < rte.pnts.size(); i++)
	{
		create_rtept_node(rte.pnts[i], rtePntNode);	
		rteNode.append_copy(rtePntNode);
	}

	return 0;
}

int GPXHelper::create_rtept_node(const RoutePoint &rtept, xml_node &rtePntNode)
{
	xml_document doc;

	// Add route point node
	rtePntNode = doc.append_child("rtept");

	// Add longitude and latitude attribut 
	rtePntNode.append_attribute("lon") = rtept.coor.lon;
	rtePntNode.append_attribute("lat") = rtept.coor.lat;

	// Add Route point name
	xml_node descr = rtePntNode.append_child("name");
	descr.append_child(pugi::node_pcdata
		).set_value(rtept.name.c_str());
	
	return 0;	
}

int GPXHelper::create_trkpt_node(const Coordinate &coord, xml_node &trkPntNode)
{
	xml_document doc;

	// Add track point
	trkPntNode = doc.append_child("trkpt");

	// Add longitude and latitude attribut 
	trkPntNode.append_attribute("lon") = coord.lon;
	trkPntNode.append_attribute("lat") = coord.lat;
	
	// Add UTC time info
	xml_node descr = trkPntNode.append_child("time");
	descr.append_child(pugi::node_pcdata).set_value(coord.utc.c_str());

	return 0;
}

string GPXHelper::to_string(const xml_node &node)
{
	ostringstream os;

	node.print(os, "", pugi::format_raw);
	//node.print(os, "\t");
	return os.str();
}

int GPXHelper::save_route(const string &path, 
	const string &fileName, const xml_node &rteNode)
{	
	string rteFile(path);
	rteFile.append(fileName);		

	// Load route file to xml document object
	xml_document doc;
	if(!doc.load_file(rteFile.c_str()))
	{
		FILE_LOG(logERROR) << "GPX: Failed to load route file";
		return -1;
	}

	FILE_LOG(logINFO) << to_string(doc);
	// Remve route node from route file
	xml_node gpxNode = doc.child("gpx");		
	FILE_LOG(logINFO) << to_string(gpxNode);
	gpxNode.remove_child("rte");	
	FILE_LOG(logINFO) << to_string(gpxNode);
	
	// Add new route node to route file
	gpxNode.append_copy(rteNode);
	FILE_LOG(logINFO) << to_string(gpxNode);

	// Save doc object
	doc.save_file(rteFile.c_str());

	return 0;
}

int GPXHelper::save_track_point(const string &path, 
	const string &fileName, const xml_node &trkptNode)
{
	string trkFile(path);
	trkFile.append(fileName);		

	// Load track file to xml document object
	xml_document doc;
	if(!doc.load_file(trkFile.c_str()))
	{
		FILE_LOG(logERROR) << "Net: Failed to load track file";
		return -1;
	}

	// Add new track point node to track file
	xml_node trksegNode = doc.child("gpx").child("trk").child("trkseg");		
	trksegNode.prepend_copy(trkptNode);

	// Save doc object
	doc.save_file(trkFile.c_str());

	return 0;
}

int GPXHelper::remove_route(const string &path, 
	const string &fileName)
{
	string rteFile(path);
	rteFile.append(fileName);		

	// Load route file to xml document object
	xml_document doc;
	if(!doc.load_file(rteFile.c_str()))
	{
		FILE_LOG(logERROR) << "Net: Failed to load route file";
		return -1;
	}

	// Remve route node from route file
	xml_node gpxNode = doc.child("gpx");		
	gpxNode.remove_child("rte");	
	
	// Save doc object
	doc.save_file(rteFile.c_str());

	return 0;
}

//
//	GPXHelper gpxHlpr;
//	gpxHlpr.create_gpx_file(GPX_PATH, GPX_ROUTE_NAME, 
//		GPX_TYPE_ROUTE);
//	gpxHlpr.create_gpx_file(GPX_PATH, GPX_TRACK_NAME, 
//		GPX_TYPE_TRACK);
//
int GPXHelper::create_gpx_file(const string &path, 
	const string &fileName, const string &type)
{
	string file(path);
	file.append(fileName);

	xml_document doc;

	// Add file declaration
	xml_node decl = doc.prepend_child(node_declaration);
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "UTF-8";
	decl.append_attribute("standalone") = "no";

	// Add gpx node
	xml_node gpxNode = doc.append_child("gpx");

	if(type.compare(GPX_TYPE_ROUTE) == 0)
	{
		gpxNode.append_child("rte");
	}
	else if(type.compare(GPX_TYPE_TRACK) == 0)
	{
		xml_node trkNode = gpxNode.append_child("trk");
		trkNode.append_child("name");
		trkNode.append_child("trkseg");
	}

	// Create new gpx file
	doc.save_file(file.c_str());

	return 0;
}

