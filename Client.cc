#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <string>
#include <strings.h>
#include "Client.h"
#include "log.h"
#include "Config.h"

using namespace std;

Client::Client():sockfd(-1)
{

}

Client::~Client()
{
	if(sockfd != -1)
	{
		close(sockfd);
	}
}

int Client::connect2srv(const string ipAddr, const int port)
{
	struct sockaddr_in srvAddr;
	char flag;

	// Create socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		FILE_LOG(logERROR) << "Net: socket create error";
		return -1;
	}
	// Disable buffering data
	setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, 
		(char *) &flag, sizeof(int)); 

	// Set server address 
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(port);
	srvAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
	bzero(&(srvAddr.sin_zero), 8); 

	// Connect to server
	if(connect(sockfd, (struct sockaddr *)&srvAddr, 
		sizeof(struct sockaddr)))
	{
		FILE_LOG(logERROR) << "Net: Connection error";
		return -1;
	}

	return 0;
}

int Client::send_data(const string &buf)
{
	return send(sockfd, buf.c_str(), buf.size(), 0);
}
	
int Client::recv_data(string &buf)
{
	char cBuf[MAX_MSG_SIZE];
	int size;

	// Receive data 
	size = recv(sockfd, &cBuf[0], MAX_MSG_SIZE, 0);	
	if(size > 0)
	{
		// Copy received data
		buf.copy(&cBuf[0], size);	
	}

	return size;
}

int Client::disconnect()
{
	close(sockfd);
	sockfd = -1;
}
