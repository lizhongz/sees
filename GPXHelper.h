#ifndef _GPXHELPER_H_
#define _GPXHELPER_H_

#include <iostream>
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
	int create_rte_node(const Route &rte, 
		xml_node &parentNode, xml_node &rteNode);

	int create_rtept_node(const RoutePoint &rtept, 
		xml_node &parentNode, xml_node &rteNode);	

	int create_trkpt_node(const Coordinate &coord, 
		xml_node &parentNode, xml_node &trkpnNode);

	string to_string(xml_node &node);
};

#endif // _GPXHELPER_H_
 
