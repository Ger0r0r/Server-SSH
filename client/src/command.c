#include "../headers/client.h"

int Command_exit(connection * data){
	char message[MAX_COMMAND_LENGHT] = {0};

	sprintf(message, "@Disconeccted");

	sendto(data->sock_fd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr)&data->admin, sizeof(&data->admin));

	return 0;
}

int Command_login(connection * data, char * log_pas){

	// char * lof_pas = "__LOGIN__:__PASSWORD__"

	char message[MAX_COMMAND_LENGHT] = {0};

	sprintf(message, "@Log in:%s", log_pas);

	sendto(data->sock_fd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr)&data->admin, sizeof(&data->admin));

	int n;
	unsigned int len;
	char admin_message[MAX_COMMAND_LENGHT] = {0};
	n = recvfrom(data->sock_fd, (char *)admin_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&data->admin, &len);
	admin_message[n] = '\0';

	if (strcmp(admin_message, "@Success"));{
		data->status = 2;
		int secret_info = open(".login.txt", O_CREAT, O_TRUNC, 0700);

		

		write(secret_info, log_pas, strlen(log_pas));
		write(secret_info, ":", 1);
		write(secret_info, data->info_user.key, strlen(data->info_user.key));
		write(secret_info, ":", 1);
		write(secret_info, data->info_user.IV, strlen(data->info_user.IV));

		close(secret_info);
	}else{

	}
	
}

int Command_connect(connection * data){

}