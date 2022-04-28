#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <time.h>

#define SSI struct sockaddr_in

#define MAX_COMMAND_LENGHT 1024
#define MAX_OUTPUT_LENGHT 65536
#define BROADCAST_PORT 23456
#define PUBLIC_KEY_P 20999999
#define PUBLIC_KEY_G 7
#define TIMEOUT_BEFORE_SEND_KEYS 1000000
#define MAX_USER_COUNT 100
#define BUFFD 666

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

typedef struct {
	SSI client;
	SSI admin;
	int sock_fd;
	int c_users;
	user ** users;
	char * key;
	char * iv;
}connection;

void Broadcast_scanning();
void Start_connection(SSI new_cn);
void Update_admin_info(int sigN, siginfo_t* sigInfo, void* context);

void Wait_connection(int mmm);
void Send_admin_info(int sigN, siginfo_t* sigInfo, void* context);
void New_admin_request(int sigN, siginfo_t* sigInfo, void* context);

size_t Encrypt_signal(SSI data);
SSI Translate_signal(size_t data);

void Administraitor_TCP(connection * bfd);
void Administraitor_UDP(connection * bfd);

connection * Get_database();
user * Get_user(char * data);
void Get_message(connection * bfd, char * message);
int Parser(char * message, char * content, connection * bfd);

int Disconnected(connection * bfd);
int Login(connection * bfd, char * log_pas);
int Check_previos_session(connection * bfd, char * data);
int Do_usual(connection * bfd, char * command);
int Copy_to(connection * bfd, char * message);
int Copy_from(connection * bfd, char * message);
void close_all_pipes(int (*massive)[2], int size);

int getFileSize(int fd);
char* file2arr(int fd, int* arrLen);
int numWords(char* arr,int lenArr);
int getWordLen(char* arr, int position, int arrLen);
int nextWord(char* arr, int position, int arrLen);
char** arr2matrix(char* arr,int lenArr, int* pSize); // Также добавляет Null перед каждой |
void printMatrix(char** matrix, int size);
int numOfBarArr(char *arr, int len); //возвращает количество |
int numOfBarMatrix(char** matrix, int size);
int lastBarArr(char *arr, int len); //Находит последний | в массиве
int lastBarMatrix(char ** matrix, int size);
void freeMatrix(char** matrix, int realSize);

void Preparing_numeral_keys(int sock_fd, SSI client, size_t * K1, size_t * K2);
void Encryption();
void Decoding();
size_t Speed_degree_with_mod(size_t g, size_t x, size_t p);
void Make_keys(size_t K1, size_t K2, char * key, char * IV);

#endif