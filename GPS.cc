#include <fcntl.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include "GPS.h"
#include "log.h"

using namespace std;

#define O_FLAGS	(O_RDONLY | O_NOCTTY | O_NDELAY)

// test
vector<Coordinate> coors;

GPS::GPS(char *path)
	:dev(path, O_FLAGS)
{
	// test
	coors.push_back(Coordinate("ab",45.75963,3.110703));
	coors.push_back(Coordinate("ab",45.75981,3.111636));
	coors.push_back(Coordinate("ab",45.759574,3.110858));
	coors.push_back(Coordinate("ab",45.759361,3.111244));
	coors.push_back(Coordinate("ab",45.759286,3.111341));
	coors.push_back(Coordinate("ab",45.759226,3.111405));
	coors.push_back(Coordinate("ab",45.759159,3.111395));
	coors.push_back(Coordinate("ab",45.759134,3.11147));
	coors.push_back(Coordinate("ab",45.759041,3.11147));
	coors.push_back(Coordinate("ab",45.758783,3.111652));
	coors.push_back(Coordinate("ab",45.758672,3.111786));
	coors.push_back(Coordinate("ab",45.758481,3.111896));
	coors.push_back(Coordinate("ab",45.758266,3.112092));
	coors.push_back(Coordinate("ab",45.758562,3.113028));
	coors.push_back(Coordinate("ab",45.758624,3.11306));
	coors.push_back(Coordinate("ab",45.758614,3.113264));
	coors.push_back(Coordinate("ab",45.758828,3.113744));
	coors.push_back(Coordinate("ab",45.758843,3.113898));
}

GPS::~GPS()
{

}

int GPS::locate(Coordinate &coor)
{
	// test
	static int i = 0;
	coor = coors[i++];
	if(i == coors.size())
	{
		i = 0;
	}
	return 0;
		
	string msg;
	RMC_data rmcData;
	
	// Read positioning message of RMC format
	read_rmc_msg(msg);
	FILE_LOG(logINFO) << msg;

	// Parse RMC message
	parse_rmc(msg, rmcData); 	

	if(rmcData.status == 'V')
	{
		FILE_LOG(logWARNING) << "GPS: not valid data";
		//return -1;
	}
	
	// Coordinate format convertion
	coor.utc = rmcData.utc;
	coor.lat = rmcData.latitude;
	coor.lon = rmcData.longitude;
	coor_format_convert(coor.lat, rmcData.dir_ns, 
		coor.lon, rmcData.dir_ew);

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
		while((dev.read_data(&ch, 1) < 1) || (ch != '$'))
		{
			;	
		}

		// Read the GNSS data head
		for(int i = 0; i < 5; i++)
		{
			if((dev.read_data(&ch, 1) == 1 && ch !='\005'))
			{
				msg.push_back(ch);	
			}
		}		

		// If the GNSS data is RMC data
		if(msg.compare("GPRMC") == 0)
			break;
		msg.clear();
	}

	// Read positioning info
	while(dev.read_data(&ch, 1) == 1 && ch != '\n')	
	{
		if(ch != '\005')
		{
			msg.push_back(ch);	
		}
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
