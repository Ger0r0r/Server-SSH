#include "../headers/client.h"

void Generetion_keys(int mode, connection * data){
	char * hey = "@Hey admin!\n";

	if (mode){
		connect(data->sock_fd, (SA*)&data->admin, sizeof(data->admin));
	}else{
		sendto(data->sock_fd, (const char *)hey, strlen(hey), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));
	}

	size_t K1, K2;
	Preparing_numeral_keys(data->sock_fd, data->admin, &K1, &K2);
	char * Key_new = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	char * IV_new = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	Make_keys(K1, K2, Key_new, IV_new);
	data->key = Key_new;
	data->iv = IV_new;
	//printf("In data\n%s %s\n", data->info_user.key, data->info_user.IV);
}