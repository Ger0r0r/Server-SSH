#include "../headers/client.h"

input Read_input(){
	char * enter = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	input ret = {.cmd = {0}, .arg = {0}};

	fgets(enter, MAX_COMMAND_LENGHT, stdin);

	enter[strlen(enter) - 1] = '\0';

	int size_command = 0;
	char * space = strchr(enter, ' ');

	if (space == NULL){
		strcpy(ret.cmd, enter);
	}else{
		size_command = space - enter;
		strncpy(ret.cmd, enter, size_command);
		strncpy(ret.arg, enter + size_command + 1, MAX_COMMAND_LENGHT - 1 - size_command);	
	}
	return ret;
}

int Do_task(connection * data, input enter){
	int ret;

	if (strcmp("@exit",enter.cmd) == 0){
		return Command_exit(data);
	}else if (strcmp("@login",enter.cmd) == 0){
		Command_login(data, enter.arg);
		ret = 0;
	}else if (strcmp("@connect",enter.cmd) == 0){
		Command_connect(data);

		if (data->status){
			printf("\n\n\tConnection already exists\n\n");
		}else{
			printf("\n\tTry to find connection...\n\n");



			data->status = Connection_attempt(data->my, &data->admin);



			if (data->status){
				char * hey = "@Hey admin!\n";
				sendto(data->sock_fd, (const char *)hey, strlen(hey), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));

				size_t K1, K2;
				Preparing_numeral_keys(data->sock_fd, data->admin, &K1, &K2);
				char Key_new[MAX_COMMAND_LENGHT] = {0};
				char IV_new[MAX_COMMAND_LENGHT] = {0};
				Make_keys(K1, K2, Key_new, IV_new);



				int res_check = Check_for_old_keys(data);
				if (res_check < 0){

					Log_in_session(data);

					///* USE ENCRYPT*/ sendto(data->sock_fd, (const char *)first_ses, strlen(first_ses), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));
				}else{
					char another_ses[MAX_COMMAND_LENGHT] = {0};
					sprintf(another_ses, "@Have previos session:%s#%s#%s#%s\n", data->info_user.login, data->info_user.password, data->info_user.key_old, data->info_user.IV_old);
					/* USE ENCRYPT*/ sendto(data->sock_fd, (const char *)another_ses, strlen(another_ses), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));
				}
			}
		}
		
		ret = 1;
	}else{
		printf("Unknown command!\n");
		ret = -1;
	}
}

int Check_for_old_keys(connection * data){
	int secret_txt = open(".login.txt", O_CREAT, 0700);

	char buf[MAX_COMMAND_LENGHT] = {0};

	// in .login.txt "__LOGIN__#__PASSWORD__#__KEY__#__IV__"

	int count = read(secret_txt, buf, MAX_COMMAND_LENGHT);
	if (count < 0){
		perror("Read secret.txt");
	}else if (count > 32){
		char * get_password = strchr(buf, '#');
		char * get_key_old = strchr(get_password + 1, '#');
		char * get_IV_old = strchr(get_key_old + 1, '#');

		buf[get_password - buf] = '\0';
		buf[get_key_old - buf] = '\0';
		buf[get_IV_old - buf] = '\0';

		strcat(data->info_user.login, buf);
		strcat(data->info_user.password, get_password + 1);
		strcat(data->info_user.key_old, get_key_old + 1);
		strcat(data->info_user.IV_old, get_IV_old + 1);

		printf("Read:\n%s\n%s\n%s\n%s\n", data->info_user.login, data->info_user.password, data->info_user.key_old, data->info_user.IV_old);
		return 1;		

	}else{
		printf("Previos session keys does'n exists\n");
		close(secret_txt);
		return -1;		
	}

}


void Log_in_session(data){
	


}