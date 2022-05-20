#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <math.h>
#include <poll.h>
#include <time.h>
#include <errno.h>
#include <termios.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <pwd.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define PPP printf("CHECK\n");

static int log_fd = -1;
#define LOG_SIZE (1 << 14)
static char buf_log[LOG_SIZE];

#define log(fmt, ...) print_log("%s:%d \n" fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define log_info(fmt, ...) log("[INFO] " fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) log("[ERROR] " fmt, ##__VA_ARGS__)
#define log_perror(fmt, ...) log(fmt " (errno = %d): %s ", errno, strerror(errno), ##__VA_ARGS__)
#define CHECK(arg) 	\
	if (errno != 0){ 				\
		log("value < 0");	\
		exit(EXIT_SUCCESS);		\
	} 							\

#define SSI struct sockaddr_in
#define SA struct sockaddr

#define MAX_COMMAND_LENGHT 65536
#define MAX_OUTPUT_LENGHT 65536
#define OWN_PORT 25575
#define BROADCAST_PORT 23456
#define COUNT_OF_ATTEMPT_FOR_CONNECTION 5
#define TIME_BETWEEN_ATTEMPTS_FOR_CONNECTION 1000000
#define SEARCHING_SERVERS_TIME 10000
#define NEED_BIND 1
#define NOT_NEED_BIND 2

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

input Read_input();
int Do_task(connection * data, input enter);
int Socket_config(struct sockaddr_in* server, uint16_t port, int socket_type, int setsockopt_option, char is_bind_need, in_addr_t addr);

int Disconnected(connection * bfd);
int Command_exit(connection * data);
int Command_exec(connection * data, input enter);
int Command_copy_to(connection * data, char * message);
int Command_copy_from(connection * data, char * message);

int Broadcast_find();
void Generetion_keys(int mode, connection * data);
int Check_for_old_keys(connection * data);
int Auto_login(connection * data);

void Send_message(int mode, connection * bfd, char * string);
void Get_message(int mode, connection * bfd, char * string);

void Preparing_numeral_keys(int mode, connection * bfd, size_t * K1, size_t * K2);
size_t Speed_degree_with_mod(size_t g, size_t x, size_t p);
void Make_keys(size_t K1, size_t K2, char * key, char * IV);
int Encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
int Decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);

#endif