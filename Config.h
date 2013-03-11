#ifndef _CONFIG_H_
#define _CONFIG_H_

// Network
#define BACKLOG		10
#define MAX_MSG_SIZE	10000
#define SERVER_PORT	4000
#define SERVER_ADDR	"127.0.0.1"

// SEES Communication Protocol
#define PROT_NAME	"SEES"
#define PROT_NAME_SIZE	(sizeof(PROT_NAME) - 1)
#define CMD_NVIGT	"1"
#define CMD_POSITION	"2"
#define CMD_CANCEL	"3"
#define CMD_FINISH	"4"
#define CMD_SIZE	(sizeof(CMD_FINISH) - 1)
#define PROT_HEAD_SIZE	(PROT_NAME_SIZE + CMD_SIZE)

// SEES Client
// Time interval for sending message to server
#define SEND_TIME_INTVL	3000 

#endif // _CONFIG_H_

