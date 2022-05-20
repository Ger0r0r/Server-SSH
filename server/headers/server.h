#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
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
#include <errno.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <time.h>
#include <pwd.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>

#define PPP printf("CHECK\n");

static int log_fd = -1;
#define LOG_SIZE (1 << 14)
static char buf_log[LOG_SIZE];

#define log(fmt, ...) print_log("%s:%d \n" fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define log_info(fmt, ...) log("[INFO] " fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) log("[ERROR] " fmt, ##__VA_ARGS__)
#define log_perror(fmt, ...) log(fmt " (errno = %d): %s ", errno, strerror(errno), ##__VA_ARGS__)
#define log_error_handler(fmt, ...) log_perror(fmt, ##__VA_ARGS__);exit(EXIT_FAILURE)

#define SSI struct sockaddr_in
#define SA struct sockaddr

#define MAX_COMMAND_LENGHT 65536
#define MAX_OUTPUT_LENGHT 65536
#define BROADCAST_PORT 23456
#define PUBLIC_KEY_P 20999999
#define PUBLIC_KEY_G 7
#define TIMEOUT_BEFORE_SEND_KEYS 1000000
#define MAX_USER_COUNT 100
#define BUFFD 666
#define TOSTRING(x) #x

typedef struct {
	char cmd[MAX_COMMAND_LENGHT];
	char arg[MAX_COMMAND_LENGHT];
}input;

typedef struct {
	SSI client;
	SSI admin;
	int sock_fd;
	int conn_fd;
	char * key;
	char * iv;
}connection;

// broadcast
void Broadcast_scanning();
void Update_admin_info(int sigN, siginfo_t* sigInfo, void* context);

// connection_handler
void Wait_connection(int connection_mode, int stdin_copy, int stdout_copy);
void Send_admin_info(int sigN, siginfo_t* sigInfo, void* context);
void New_admin_request(int sigN, siginfo_t* sigInfo, void* context);

// administaitor
void Administraitor(int mode, int stdin_copy, int stdout_copy);

// tasks
int Disconnected(connection * bfd);
int Do_usual(connection * bfd, char * command);
int Copy_to(connection * bfd, char * message);
int Copy_from(connection * bfd, char * message);

// message
void Send_message(int mode, connection * bfd, char * string);
void Get_message(int mode, connection * bfd, char * string);

// log
int print_time();
int init_log(char* path);
void print_log(char* str, ...);
void printf_fd(int fd, char* str, ...);

// signal_operation
SSI Translate_signal(size_t data);
size_t Encrypt_signal(SSI data);

// encrypting
void Preparing_numeral_keys(int mode, connection * bfd, size_t * K1, size_t * K2);
int Encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
int Decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);
size_t Speed_degree_with_mod(size_t g, size_t x, size_t p);
void Make_keys(size_t K1, size_t K2, char * key, char * IV);

// users
int Login_into_user(char *username);
int User_login(char *username);

#endif