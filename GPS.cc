#include <fcntl.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include "GPS.h"

using namespace std;

#define O_FLAGS	(O_RDONLY | O_NOCTTY | O_NDELAY)

GPS::GPS(char *path)
	:dev(path, O_FLAGS)
{

}

GPS::~GPS()
{

}

int GPS::positioning(Coordinate &coor)
{
	string msg;
	RMC_data rmcData;
	
	// Read positioning message of RMC format
	read_rmc_msg(msg);
	cout << msg << endl;

	// Parse RMC message
	parse_rmc(msg, rmcData); 	
	
	// Coordinate format convertion
	coor.utc = rmcData.utc;
	coor.lat = rmcData.latitude;
	coor.lon = rmcData.longitude;
	coor_format_convert(coor.lat, rmcData.dir_ns, coor.lon, rmcData.dir_ew);

	return 0;
}

void GPS::read_rmc_msg(string &msg)
{
	char ch;

	msg.clear();

	// Read only recommended minimum specific GNSS data(RMC)
	while(1)
	{
		// Move to a beginning position
		do {
			dev.uart_read(&ch, 1);
		} while(ch != '$');

		// Read the GNSS data head
		for(int i = 0; i < 5; i++)
		{
			dev.uart_read(&ch, 1);
			msg.push_back(ch);	
		}		

		// If the GNSS data is RMC data
		if(msg.compare("GPRMC") == 0)
			break;
		msg.clear();
	}

	// Read positioning info
	while(dev.uart_read(&ch, 1) == 1 && ch != '\r')	
	{
		msg.append(&ch);	
	}
}

void GPS::parse_rmc(const string &rawMsg, RMC_data &rmcData)
{
	vector<string> splMsgs;
	char delim = ',';

	// Split raw message by ','
	split(rawMsg, delim, splMsgs);

	// convert splitted raw message to RMC data
	rmcData.utc	= splMsgs[1];	
	rmcData.status	= (char)splMsgs[2][0];
	rmcData.latitude = atof(splMsgs[3].c_str());
	rmcData.dir_ns	= splMsgs[4][0];
	rmcData.longitude = atof(splMsgs[5].c_str());
	rmcData.dir_ew	= splMsgs[6][0];
#if 0
	cout <<  rmcData.utc << endl;
	cout <<  rmcData.status << endl;
	cout <<  rmcData.latitude  << endl;
	cout <<  rmcData.dir_ns << endl;
	cout <<  rmcData.longitude << endl;
	cout <<  rmcData.dir_ew << endl;
#endif
}

void GPS::split(const string &rawMsg, const char delim, vector<string> &splMsgs)
{
	string tmpStr("");

	splMsgs.clear();

	// Splilt raw messege into array by delim
	
	for(int i = 0; i < rawMsg.size(); i++)
	{	
		// If not delimiter
		if(rawMsg[i] != delim)
		{
			// If not '\005'(Enquiry character)
			if(rawMsg[i] != '\005')
			{
				tmpStr.push_back(rawMsg[i]);
			}
		}
		else
		{
			if(tmpStr.size() != 0)
			{
				splMsgs.push_back(tmpStr);
				tmpStr.clear();
			}
		}
	}

	if(tmpStr.size() != 0)
	{
		splMsgs.push_back(tmpStr);
	}
}

void GPS::coor_format_convert(double &lat, char ns, double &lon, char ew)
{
	int degree;
	double min; 

	// Convert latitue from ddmm.mmmm format to singed degrees format	
	degree	= lat / 100;
	min	= lat - degree * 100;
	lat	= degree + min / 60;
	if(ns == 'S')
		lat *= -1;	

	// Convert longitute from dddmm.mmmm format to singed degrees format	
	degree	= lon / 100;
	min	= lon - degree * 100;
	lon	= degree + min / 60;
	if(ns == 'W')
		lon *= -1;	
}
