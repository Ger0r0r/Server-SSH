#include "../headers/server.h"

int Disconnected(){
	return 0;
}

int Login(connection * bfd, char * log_pas){
	// log_pas "__LOGIN__#__PASSWORD__"

	char username[MAX_COMMAND_LENGHT] = {0};
	char password[MAX_COMMAND_LENGHT] = {0};

	char * border = strchr(log_pas, '#');
	border[0] = '\0';
	strcpy(username, log_pas);
	strcpy(password, border + 1);

	for (size_t i = 0; i < bfd->c_users; i++){
		if ((strcmp(bfd->users[i]->login, username) == 0) && (strcmp(bfd->users[i]->password, password) == 0)){
			char message[MAX_COMMAND_LENGHT] = "@Success";
			/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));
			bfd->users[i]->IV = bfd->iv;
			bfd->users[i]->key = bfd->key;
			return 1;
		}
	}
	printf("Wrong login or password!\n");
	char message[MAX_COMMAND_LENGHT] = "@Wrong username or password!";
	/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));
	return 1;	
}

int Check_previos_session(connection * bfd, char * data){
	// @Have_previos_session __LOGIN__#__PASSWORD__#__OLD_KEY__#__OLD_IV__

	char * border = strchr(data, '#');
	char * border_start = strchr(border + 1, '#');
	char * border_between = strchr(border_start + 1, '#');
	border[0] = '\0';
	border_start[0] = '\0';
	border_between[0] = '\0';

	char old_key[MAX_COMMAND_LENGHT] = {0};
	char old_iv[MAX_COMMAND_LENGHT] = {0};

	strcpy(old_key, border_start+1);
	strcpy(old_iv, border_between+1);

	for (size_t i = 0; i < bfd->c_users; i++){
		if ((strcmp(bfd->users[i]->key_old, old_key) == 0) && (strcmp(bfd->users[i]->IV_old, old_iv) == 0)){
			char message[MAX_COMMAND_LENGHT] = "@Success";
			/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));
			bfd->users[i]->IV = bfd->iv;
			bfd->users[i]->key = bfd->key;
			return 1;
		}
	}
	char message[MAX_COMMAND_LENGHT] = "@Wrong";
	/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));
	return 1;	
}

int Do_usual(connection * bfd, char * command){

}

int Copy_to(){

}

int Copy_from(){

}
