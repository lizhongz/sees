#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>

using namespace std;

class Client
{
private:
	int sockfd;
public:
	Client();

	~Client();

	// Connect to server
	int connect2srv(const string ipAddr, const int port);

	// Send data to server
	int send_data(const string &buf);
	
	// Receive data from server
	int recv_data(string &buf);
	
	// Disconnect from server
	int disconnect();
};


#endif
