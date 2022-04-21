#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>       


#define MAX_COMMAND_LENGHT 1024
#define PORT	 8080

typedef struct {
	char cmd[MAX_COMMAND_LENGHT];
	char arg[MAX_COMMAND_LENGHT];
}input;

typedef struct {
	in_addr addr;
	in_port_t port;
}connection;

void Broadcast_scanning();
void Wait_connection();
void Handler_connection(int sigN, siginfo_t* sigInfo, void* context);
connection Translate_signal(size_t data);
void Start_connection(struct sockaddr_in new_cn)

#endif