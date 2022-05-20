#include "../headers/server.h"

void Send_message(int mode, connection * bfd, char * string){
	//printf("CHECK\n");
	
	char new_string[MAX_COMMAND_LENGHT] = {};

	Encrypt(string, strlen(string), bfd->iv, bfd->key, new_string);

	printf("CHECK\n");
	if (mode){
		write(bfd->conn_fd, new_string, strlen(new_string));	
	}else{
		sendto(bfd->sock_fd, (const char *)new_string, strlen(new_string), MSG_CONFIRM, (const struct sockaddr *)&(bfd->client), sizeof(bfd->client));		log_perror("sendto in Prepare keys");
	}
}

void Get_message(int mode, connection * bfd, char * string){
	memset(string, '\0', MAX_COMMAND_LENGHT);

	char new_string[MAX_COMMAND_LENGHT] = {};

	int n;
	unsigned int len;
	//printf("Wait message))\n");
	if (mode){
		n = read(bfd->conn_fd, new_string, MAX_COMMAND_LENGHT);
	}else{
		n = recvfrom(bfd->sock_fd, (char *)new_string, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&(bfd->client), &len);		log_perror("recvfrom in Prepare keys");
	}
	//printf("recieve message:\n%s\n", new_string);

	Decrypt(new_string, strlen(new_string), bfd->iv, bfd->key, string);

	printf("decrypt message:\n%s\n", string);
}