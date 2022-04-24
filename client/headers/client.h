#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COMMAND_LENGHT 1024
#define OWN_PORT 25575
#define BROADCAST_PORT 23456
#define COUNT_OF_ATTEMPT_FOR_CONNECTION 10
#define TIME_BETWEEN_ATTEMPTS_FOR_CONNECTION 10000

typedef struct {
	char cmd[MAX_COMMAND_LENGHT];
	char arg[MAX_COMMAND_LENGHT];
}input;

input Read_input();
int Do_task(input enter);

struct sockaddr_in Get_addr();
void Bind_addr(int sock_fd_own, struct sockaddr_in own_addr);

int Connection_attempt();
int Broadcast_find();


#endif