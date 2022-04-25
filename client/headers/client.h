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
#include <time.h>

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

typedef struct {
	char * login;
	char * password;
	char * key;
	char * IV;
	char * key_old;
	char * IV_old;
}user;

typedef struct{
	SSI admin;
	SSI my;
	int sock_fd;
	int status; // 2 - login; 1 - connected; 0 - disconnected
	user info_user;
}connection;

input Read_input();
int Do_task(connection * data, input enter);

int Command_exit(connection * data);
int Command_login(connection * data, char * log_pas);
int Command_connect(connection * data);
int Command_exec(connection * data, input enter);

int Connection_attempt(SSI own_addr, SSI * adm_addr);
int Broadcast_find(SSI own_addr, SSI * ret_addr);
void Generetion_keys(data);
int Check_for_old_keys(connection * data);
void Auto_login(connection * data);

void Preparing_numeral_keys(int sock_fd, SSI admin, size_t * K1, size_t * K2);
size_t Speed_degree_with_mod(size_t g, size_t x, size_t p);
void Make_keys(size_t K1, size_t K2, char * key, char * IV);
void Encryption();
void Decoding();


#endif