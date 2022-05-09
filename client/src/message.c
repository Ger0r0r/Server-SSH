#include "../headers/client.h"

void Send_message(int mode, connection * bfd, char * string){
	
	char new_string[MAX_COMMAND_LENGHT] = {};

	Decrypt(string, strlen(string), bfd->iv, bfd->key, new_string);

	if (mode){
		write(bfd->conn_fd, new_string, strlen(new_string));	
	}else{
		sendto(bfd->sock_fd, (const char *)new_string, strlen(new_string), MSG_CONFIRM, (const struct sockaddr *)&(bfd->admin), sizeof(bfd->admin));
	}
}

void Get_message(int mode, connection * bfd, char * string){
	memset(string, '\0', MAX_COMMAND_LENGHT);

	char new_string[MAX_COMMAND_LENGHT] = {};

	int n;
	unsigned int len;
	if (mode){
		n = read(bfd->conn_fd, new_string, MAX_COMMAND_LENGHT);
	}else{
		n = recvfrom(bfd->sock_fd, (char *)new_string, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&(bfd->admin), &len);
	}

	Decrypt(new_string, strlen(new_string), bfd->iv, bfd->key, string);
}