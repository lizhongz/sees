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
	int uart_read(char *data, int len);
	// write data from uart port
	int uart_write(const char *data, const int len);
};

#endif
