// GPS driver class for GPS device GR-213

#ifndef _GPS_H_
#define _GPS_H_

#include <string>
#include <vector>
#include "Uart.h"
#include "Coordinate.h"

using namespace std;

// RMC data 
typedef struct
{
	string	utc;		// UTC time
	char	status;		// Data status(A = valid, V= not valid
	double	latitude;	// Latitude of fix
	char	dir_ns;		// Direction: N or S
	double	longitude;	// Longitude od fix	
	char	dir_ew;		// Direction: E or W
}RMC_data;

class GPS
{
private:
	Uart dev; // GPS Device 

	// Read positioning message of RMC format
	void read_rmc_msg(string &msg);

	// Parse RMC message
	void parse_rmc(const string &rawMsg, RMC_data &rmcData);

	// Split raw messege into string array by delim
	void split(const string &rawMsg, const char delim, vector<string> &splMsgs);
	
	
	// Convert coordinate format from ddmm.mmmm format to singed degrees format	
	void coor_format_convert(double &lat, char ns, double &lon, char ew);

public:
	GPS(char *path);
	~GPS();

	// Positioning an return a coordinate
	int positioning(Coordinate &coor);
};

#endif
