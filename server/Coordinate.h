
#ifndef _COORDINATE_H_ 
#define _COORDINATE_H_

#include <string>

using namespace std;

class  Coordinate
{
public:
	string	utc;	// UTC time
	double	lat;	// latitude
	double	lon;	// longitude

	Coordinate();

	Coordinate(string utc, double lat, double lon);

	~Coordinate();
};

#endif
