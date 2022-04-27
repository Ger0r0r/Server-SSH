#include "../headers/client.h"

/*
	COMMANDS
	-3	- connection failed
	-2	- log in failed
	-1	- error
	0	- exit
	1	- log in success
	2	- connection success
*/

int Command_exit(connection * data){
	char message[MAX_COMMAND_LENGHT] = {0};

	sprintf(message, "@Disconeccted");

	sendto(data->sock_fd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(&data->admin));

	return 0;
}

int Command_login(connection * data, char * log_pas){

	// char * lof_pas = "__LOGIN__#__PASSWORD__"

	char message[MAX_COMMAND_LENGHT] = {0};

	sprintf(message, "@Log_in %s", log_pas);

	/**/sendto(data->sock_fd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(&data->admin));

	int n;
	unsigned int len;
	char admin_message[MAX_COMMAND_LENGHT] = {0};
	/**/n = recvfrom(data->sock_fd, (char *)admin_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&data->admin, &len);
	admin_message[n] = '\0';

	if (strcmp(admin_message, "@Success")){
		data->status = 2;
		int secret_info = open(".login.txt", O_CREAT, O_TRUNC, 0700);

		dprintf(secret_info, "%s:%s:%s\n", log_pas, data->info_user.key, data->info_user.IV);

		close(secret_info);
		return 1;
	}else{
		return 2;
	}
	return -1;
}

int Command_connect(connection * data){
	if (data->status){
		printf("\n\n\tConnection already exists\n\n");
	}else{
		printf("\n\tTry to find connection...\n\n");

		data->status = Connection_attempt(data->my, &data->admin);

		if (data->status){

			Generetion_keys(data);

			int res_check = Check_for_old_keys(data);
			if (res_check > 0){
				return Auto_login(data);
			}else{
				return data->status;
			}
		}
	}
	return -1;
}

int Command_exec(connection * data, input enter){
	char message[MAX_COMMAND_LENGHT] = {0};
	sprintf(message, "@#%s %s", enter.cmd, enter.arg);
	/**/sendto(data->sock_fd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));

	int n;
	unsigned int len;
	char admin_message[MAX_COMMAND_LENGHT] = {0};
	/**/n = recvfrom(data->sock_fd, (char *)admin_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&data->admin, &len);
	admin_message[n] = '\0';

	printf("\n%s\n", admin_message);
}