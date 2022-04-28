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
	char * message = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	sprintf(message, "@Disconnected");
	sendto(data->sock_fd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));
	free(message);
	return 0;
}

int Command_login(connection * data, char * log_pas){
	// lof_pas "__LOGIN__#__PASSWORD__"

	printf("In data\n%s %s\n", data->info_user.key, data->info_user.IV);

	char message[MAX_COMMAND_LENGHT] = {0};
	sprintf(message, "@Login %s", log_pas);

	/**/sendto(data->sock_fd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&(data->admin), sizeof((data->admin)));
	perror("SENDTO SERVER LOGIN");

	int n;
	unsigned int len;
	char admin_message[MAX_COMMAND_LENGHT] = {0};
	/**/n = recvfrom(data->sock_fd, (char *)admin_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&data->admin, &len);
	admin_message[n] = '\0';

	if (strcmp(admin_message, "@Success") == 0){

		data->status = 2;
		int secret_info = open(".login.txt", O_CREAT | O_TRUNC | O_RDWR, 0700);
		perror("OPEN LOGIN.TXT");

		dprintf(secret_info, "%s#%s#%s\n", log_pas, data->info_user.key, data->info_user.IV);
		perror("DPRINTF TO LOGIN.TXT");
		close(secret_info);

		char * border = strchr(log_pas, '#');
		border[0] = '\0';
		data->info_user.login = log_pas;
		data->info_user.password = border + 1;

		/**/printf("\n\tWelcome, %s!\n\n", data->info_user.login);
		return 1;
	}else{
		return 2;
	}
	return -1;
}

int Command_connect(connection * data){
	printf("\n\tTry to find connection...\n\n");

	data->status = Connection_attempt(data->my, &data->admin);

	if (data->status){

		Generetion_keys(data);

		printf("In data\n%s %s\n", data->info_user.key, data->info_user.IV);

		int res_check = Check_for_old_keys(data);
		if (res_check > 0){
			return Auto_login(data);
		}else{
			return data->status;
		}
	}
	return -1;
}

int Command_exec(connection * data, input enter){
	char message[MAX_COMMAND_LENGHT] = {0};
	sprintf(message, "@# %s %s", enter.cmd, enter.arg);
	/**/sendto(data->sock_fd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));

	int n;
	unsigned int len;
	char admin_message[MAX_OUTPUT_LENGHT] = {0};
	/**/n = recvfrom(data->sock_fd, (char *)admin_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&data->admin, &len);
	admin_message[n] = '\0';

	printf("\n%s\n", admin_message);
	return 1;
}