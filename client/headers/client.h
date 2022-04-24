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

#define SSI struct sockaddr_in

#define MAX_COMMAND_LENGHT 1024
#define OWN_PORT 25575
#define BROADCAST_PORT 23456
#define COUNT_OF_ATTEMPT_FOR_CONNECTION 5
#define TIME_BETWEEN_ATTEMPTS_FOR_CONNECTION 1000000

typedef struct {
	char cmd[MAX_COMMAND_LENGHT];
	char arg[MAX_COMMAND_LENGHT];
}input;

typedef struct{
	SSI admin;
	SSI my;
	int status; // 1 - connected; 0 - disconnected
}connection;

input Read_input();
int Do_task(connection * data, input enter);

int Connection_attempt(SSI own_addr, SSI * adm_addr);
int Broadcast_find(SSI own_addr, SSI * ret_addr);

void Preparing_numeral_keys(int sock_fd, SSI admin);
void Encryption();
void Decoding();
int Speed_degree_with_mod(int g, int x, int p);


#endif