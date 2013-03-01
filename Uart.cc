#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>                             
#include <stdlib.h>
#include "Uart.h"

Uart::Uart(char *path, int oflags)
{
	open_port(path, oflags);
}

Uart::~Uart()
{
	close(fd);
}

void Uart::open_port(char *path, int oflags)
{
	struct termios options;

	// Open file
	if((fd = open(path, oflags)) == -1)
	{
		printf("Open uart port error\n");	
		exit(1);
	}

	// Intialization uart port
	
	fcntl(fd,F_SETFL,0);
	//fcntl(fd, F_SETFL, FNDELAY); //ne pas bloquer sur le read
	tcgetattr(fd,&options);
	usleep(10000);
	cfsetospeed(&options,B4800);
	cfsetispeed(&options,B4800);
	options.c_cflag &= ~PARENB; /* Parite   : none */
	options.c_cflag &= ~CSTOPB; /* Stop bit : 1    */
	options.c_cflag &= ~CSIZE;  /* Bits     : 8    */
	options.c_cflag |= CS8;
	options.c_cflag &= ~CRTSCTS;
	// options.c_iflag &= ~(IXON);
	// tcsetattr(fd,TCSANOW,&options);
	options.c_lflag &= ~OPOST;
	options.c_lflag &= ~(ICANON | ECHO | ECHONL|IEXTEN | ISIG);
	// c_cc
	options.c_cc[VMIN]= 1;
	options.c_cc[VTIME]= 4;
	//flush!
	//tcflush(fd,TCIOFLUSH);
	tcsetattr(fd, TCSANOW,&options);
	tcflush(fd,TCIOFLUSH);
	usleep(10000);
}

int Uart::uart_read(char *data, int len)
{
	int i;
	int flag;

	for(i = 0; i < len; i++)
	{
		//printf(".");		
		flag = read(fd, &data[i], 1);	
		if(flag == -1)	// Error
		{
			return flag;	
		}
		else if(flag == 0) // No available character
		{
			break;
		}
		
	}
	//tcflush(fd,TCIOFLUSH);

	return i;
}

int Uart::uart_write(const char *data, const int len)
{
	int i;
	int flag;

	for(i = 0; i < len; i++)
	{
		flag = write(fd, &data[i], 1);
		if(flag == -1) // Error
		{
			return flag;	
		}
		else if(flag == 0) // Nothing was written
		{
			break;
		}

		//usleep(20000);
	}
	tcflush(fd, TCIOFLUSH);

	return i;
}

