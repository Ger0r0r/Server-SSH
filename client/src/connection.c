#include "../headers/client.h"

void Generetion_keys(int mode, connection * data){
	char * hey = "@Hey admin!\n";
	//printf("Admin %s:%d\n", inet_ntoa(data->admin.sin_addr), htons(data->admin.sin_port));

	if (mode){
		if(connect(data->sock_fd, (SA*)&data->admin, sizeof(data->admin)) != 0){
			printf("CONNECT ERROR!!!\n");
			exit(EXIT_FAILURE);
		}
	}else{
		sendto(data->sock_fd, (const char *)hey, strlen(hey), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));
	}

	size_t K1, K2;
	Preparing_numeral_keys(mode, data, &K1, &K2);
	char * Key_new = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	char * IV_new = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	Make_keys(K1, K2, Key_new, IV_new);
	data->key = Key_new;
	data->iv = IV_new;
	//printf("In data\n%s %s\n", data->info_user.key, data->info_user.IV);
}