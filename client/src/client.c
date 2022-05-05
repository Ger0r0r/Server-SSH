#include "../headers/client.h"

int Socket_config(struct sockaddr_in* server, uint16_t port, int socket_type, int setsockopt_option, char is_bind_need, in_addr_t addr) {
    int a = 1;
    server -> sin_family = AF_INET;
    server -> sin_port   = htons(port);
    server -> sin_addr.s_addr = addr;
    int created_socket = socket(AF_INET, socket_type, 0);

    if (created_socket < 0) {
        //log_perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt_option > 0)
        if (setsockopt(created_socket, SOL_SOCKET, setsockopt_option, &a, sizeof(a)) < 0) {
            //log_perror("setsockopt");
            exit(EXIT_FAILURE);
        }

	char NEED_BIND = 1;
	char NOT_NEED_BIND = 2;
    if (is_bind_need == NEED_BIND) 
        if (bind(created_socket, (struct sockaddr*) server, sizeof(*server)) < 0) {
            if (socket_type == SOCK_DGRAM && errno == 98) {
                //log_perror("bind port problem\n");
                return -1;
            }
            //log_perror("bind port = %hu\n", port);
            exit(EXIT_FAILURE);
        }

    return created_socket;
}

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
		if (data->status == 1){
			return Command_login(data, enter.arg);
		}else if (data->status == 2){
			printf("\n\tYou already login\n");
		}else{
			printf("\n\tNo connection (use @connect)\n");
		}
		return 1;
	}else if (strcmp("@connect",enter.cmd) == 0){
		if (data->status == 0){
			return Command_connect(data);
		}else{
			printf("\n\tYou already connected\n");
		}
	}else if (strcmp("@copy_to",enter.cmd) == 0){
		if (data->status == 2){
			return Command_copy_to(data, enter.arg);
		}else if (data->status == 1){
			printf("\n\tYou must be login (use @login __LOGIN__#__PASSWORD__)\n");			
		}else{
			printf("\n\tNo connection (use @connect)\n");
		}
	}else if (strcmp("@copy_from",enter.cmd) == 0){
		if (data->status == 2){
			return Command_copy_from(data, enter.arg);
		}else if (data->status == 1){
			printf("\n\tYou must be login (use @login __LOGIN__#__PASSWORD__)\n");			
		}else{
			printf("\n\tNo connection (use @connect)\n");
		}
	}else if (enter.cmd[0] == '@'){
		printf("\n\tUnknown command!\n");
	}else{
		if (data->status == 2){
			return Command_exec(data,enter);
		}else if (data->status == 1){
			printf("\n\tYou must be login (use @login __LOGIN__#__PASSWORD__)\n");			
		}else{
			printf("\n\tNo connection (use @connect)\n");
		}
		
		
		// Command to server (get "__COMMAND__", but send "@#__COMMAND__")
	}
	return -1;
}