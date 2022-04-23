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
#define BROADCAST_PORT 23456
#define ARRAY_SIZE_FOR_0BINT 32
#define PUBLIC_KEY_P 9887
#define PUBLIC_KEY_G 7

typedef struct {
	char cmd[MAX_COMMAND_LENGHT];
	char arg[MAX_COMMAND_LENGHT];
}input;

typedef struct {
	struct in_addr addr;
	in_port_t port;
}connection;

void Broadcast_scanning();
void Start_connection(struct sockaddr_in new_cn);

void Wait_connection(int mmm);
void Handler_connection(int sigN, siginfo_t* sigInfo, void* context);
connection Translate_signal(size_t data);

void Administraitor_TCP(connection client, int ccl);
void Administraitor_UDP(connection client, int ccl);

void Preparing_keys(int sock_fd, struct sockaddr_in client);
void Encryption();
void Decoding();
int Speed_degree_with_mod(int g, int x, int p);

#endif