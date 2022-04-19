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

int Get_code(input enter){
	int ret;

	if (strcmp("exit",enter.cmd) == 0){
		ret = 0;
	}else if (strcmp("help",enter.cmd) == 0){
		ret = 1;
	}else if (strcmp("connect",enter.cmd) == 0){
		ret = 2;
	}else{
		printf("Unknown command!\n");
		ret = -1;
	}

	return ret;
}

void Do_task(input enter, connection serv_info,int code){
	switch (code){
	case 1:{ //help
		printf(
			"connect - try to connect to server\n"
			"exit - shut down connection and exit program\n"
			"help - list of possible commands\n"
		);
		break;
	}
	case 2:{ //connect

		break;
	}
	case 3:{

		break;
	}
	case 4:{

		break;
	}
	case 5:{

		break;
	}
	case 6:{

		break;
	}
	default:
		//ERROR
		break;
	}
}