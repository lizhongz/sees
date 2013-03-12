#include <string>

using namespace std;

class Server
{
private:
	int sockfd;

public:
	Server();

	~Server();

	// Create socket and bind with port
	int setup(int port);
	
	// Waiting for connection
	int wait_conn();

	// Send data to client
	int send_data(int fd, const string &data);

	// Receive data
	int recv_data(int fd, string &data);

	// Communicate with a client
	int comm_with_clt(int cltfd);

	// Handle client's requests
	virtual int handle_clt_req(int cltfd, string req) = 0;
};

