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
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define SSI struct sockaddr_in

#define MAX_COMMAND_LENGHT 1024
#define BROADCAST_PORT 23456
#define ARRAY_SIZE_FOR_0BINT 32
#define PUBLIC_KEY_P 9887
#define PUBLIC_KEY_G 7
#define TIMEOUT_BEFORE_SEND_KEYS 1000000

typedef struct {
	char cmd[MAX_COMMAND_LENGHT];
	char arg[MAX_COMMAND_LENGHT];
}input;

void Broadcast_scanning();
void Start_connection(SSI new_cn);
void Update_admin_info(int sigN, siginfo_t* sigInfo, void* context);

void Wait_connection(int mmm);
void Send_admin_info(int sigN, siginfo_t* sigInfo, void* context);
void New_admin_request(int sigN, siginfo_t* sigInfo, void* context);

size_t Encrypt_signal(SSI data);
SSI Translate_signal(size_t data);

void Administraitor_TCP();
void Administraitor_UDP();

void Preparing_numeral_keys(int sock_fd, SSI client);
void Encryption();
void Decoding();
int Speed_degree_with_mod(int g, int x, int p);

#endif