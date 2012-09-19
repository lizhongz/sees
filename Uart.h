#ifndef _UART_H_
#define _UART_H_

class Uart
{
private:
	int fd; // file descripor

	void open_port(char *path, int oflags);

public:
	Uart(char *path, int oflags);
	~Uart();
	// read data from uart port
	int read_data(char *data, int len);
	// write data from uart port
	int write_data(const char *data, const int len);
};

#endif
