#ifndef _GPXHELPER_H_
#define _GPXHELPER_H_

#include <iostream>
#include <string>
#include "pugiconfig.h"
#include "pugixml.h"
#include "Route.h"
#include "RoutePoint.h"
#include "Coordinate.h"

using namespace std;
using namespace pugi;

class GPXHelper
{
public:
	// Create a gpx file
	int create_gpx_file(const string &path, 
		const string &fileName, const string &type);

	// Create xml route node by Route object
	int create_rte_node(const Route &rte, xml_node &rteNode);

	// Create xml route point node by RoutePoint object
	int create_rtept_node(const RoutePoint &rtept, xml_node &rteNode);	

	// Create track point node by Coordinate object
	int create_trkpt_node(const Coordinate &coord, xml_node &trkpnNode);

	// Print a xml node to a string
	string to_string(const xml_node &node);

	// Save a xml route node to a gpx file
	int save_route(const string &path, 
		const string &fileName, const xml_node &rteNode);
	
	// Save a xml track point nodd to a gpx file
	int save_track_point(const string &path, 
		const string &fileName, const xml_node &trkptNode);
	
	// Remove a route from a route gpx file
	int remove_route(const string &path, 
		const string &fileName);
};

#endif // _GPXHELPER_H_
 
