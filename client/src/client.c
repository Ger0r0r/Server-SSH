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
		ret = 0;
	}else if (strcmp("@connect",enter.cmd) == 0){
		if (data->status){
			printf("\n\n\tConnection already exists\n\n");
		}else{
			printf("\n\tTry to find connection...\n\n");

			data->status = Connection_attempt(data->my, &data->admin);

			Preparing_numeral_keys(data->sock_fd, data->admin);
		}
		
		ret = 1;
	}else{
		printf("Unknown command!\n");
		ret = -1;
	}

	return ret;
}