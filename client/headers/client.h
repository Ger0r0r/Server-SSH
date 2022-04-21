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

#define MAX_COMMAND_LENGHT 1024
#define BROADCAST_PORT 23456

typedef struct {
	char cmd[MAX_COMMAND_LENGHT];
	char arg[MAX_COMMAND_LENGHT];
}input;

typedef struct {

}connection;

input Read_input();
int Get_code(input enter);
void Do_task(input enter, connection serv_inf0, int code);

#endif