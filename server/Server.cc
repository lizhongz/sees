#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <string>
#include <iostream>
#include "Server.h"
#include "Config.h"
#include "log.h"

using namespace std;

Server::Server():sockfd(-1)
{

}

Server::~Server()
{
	if(sockfd != -1)
		close(sockfd);
}

int Server::setup(int port)
{
	struct sockaddr_in srvAddr;

	// Create socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		FILE_LOG(logERROR) << "Net: socket create error";
		return -1;
	}

	// Set server socket address 
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(port);
	srvAddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(srvAddr.sin_zero), 8);

	// Bind socket to socket address 
	if(bind(sockfd, (struct sockaddr *)&srvAddr, 
		sizeof(struct sockaddr)) == -1)
	{
		FILE_LOG(logERROR) << "Net: Binding error";
		return -1;
	}

	// Listent to server socket
	if(listen(sockfd, BACKLOG) == -1)
	{
		FILE_LOG(logERROR) << "Net: Listenning error";
		return -1;
	}

	return 0;
}

int Server::wait_conn()
{
	while(1)
	{
		int cltfd;
		struct sockaddr_in cltAddr;
		unsigned int sinSize = sizeof(struct sockaddr_in); 

		// Wait for client's connection 
		if((cltfd = accept(sockfd, (struct sockaddr *)&cltAddr, 
			&sinSize)) == -1)	
		{
			FILE_LOG(logERROR) << "Net: Accepting error";
			return -1;
		}
		FILE_LOG(logDEBUG) << "Net: New client";
		
		// Communicate with new client
		comm_with_clt(cltfd);
	}		

	return 0;
}

int Server::comm_with_clt(int cltfd)
{
	while(1)
	{
		string data;
		int dataSize;

		// Receive data from client
		dataSize = recv_data(cltfd, data);

		if(dataSize == -1)
		{
			close(cltfd); // Close socket			
			FILE_LOG(logERROR) << "Net: Receiving error";
			return -1;
		}
		else if(dataSize == 0)
		{
			close(cltfd); // Close socket			
			FILE_LOG(logINFO) << "Net: Connection is dropped";
			break;
		}

		// Deal with received data
		handle_clt_req(cltfd, data);
	}

	return 0;
}

int Server::send_data(int fd, const string &data)
{
	return send(fd, data.c_str(), data.size(), 0);
}

int Server::recv_data(int fd, string &data)
{
	char cBuf[MAX_MSG_SIZE];
	int size;

	// Receive data 
	size = recv(fd, &cBuf[0], MAX_MSG_SIZE, 0);	
	if(size > 0)
	{
		cBuf[size] = '\0';
		// Copy received data
		data = cBuf;	
	}

	return size;
}
