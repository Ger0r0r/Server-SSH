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
		return Command_login(data, enter.arg);
	}else if (strcmp("@connect",enter.cmd) == 0){
		return Command_connect(data);
	}else if (strcmp("@copy_to",enter.cmd) == 0){
		//return Command_connect(data);
	}else if (strcmp("@copy_from",enter.cmd) == 0){
		//return Command_connect(data);
	}else if (enter.cmd[0] == '@')){
		printf("Unknown command!\n");
	}else{
		return Command_exec(data, enter);
		// Command to server (get "__COMMAND__", but send "@#__COMMAND__")
	}
	return -1;
}