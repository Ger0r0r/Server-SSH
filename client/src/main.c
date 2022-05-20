#include "../headers/client.h"

int mode;

int main(int argc, char ** argv) {
	// ./_PROG_NAME_ -t TCP/UDP _USER_@_IP_:_PORT_
	// ./_PROG_NAME_ -broadcast


	if (argc < 2){
		printf("Start program with task:\n%s -t (connect)\n%s -broadcast (find servers)\n", argv[0], argv[0]);
		return 0;
	}

	struct sockaddr_in server;
	int socket_ss = 0;

	if (strcmp("-broadcast", argv[1]) == 0){
		socket_ss = Socket_config(&server, BROADCAST_PORT, SOCK_DGRAM, SO_BROADCAST, NOT_NEED_BIND, htonl(INADDR_BROADCAST));
		if (Broadcast_find() == 0)
			return 0;
	}else if (strcmp("-t", argv[1]) == 0){
		if (strcmp("TCP", argv[2]) == 0){
			mode = 1;
		}else if (strcmp("UDP", argv[2]) == 0){
			mode = 0;
		}else{
			printf("Enter correct mode: TCP or UDP\ncorrect input: %s -t <TCP/UDP> <USER>@<IP:PORT>\nyou input: %s -t %s\n", argv[0], argv[0], argv[2]);
			return 0;
		}
	}else{
		printf("Start program with task:\n%s -t (connect)\n%s -broadcast (find servers)\n", argv[0], argv[0]);
		return 0;
	}

	printf("mode %d\n", mode);

	int sock_fd_own;
	if (mode){ // UDP = 0 TCP = 1
		if ((sock_fd_own = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("socket creation failed...\n");
			exit(0);
		}
	}else{
		if ((sock_fd_own = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
			perror("socket creation failed");
			exit(EXIT_FAILURE);
		}
	}

	connection con_data;
	memset(&con_data, 0, sizeof(con_data));
	con_data.sock_fd = sock_fd_own;
	con_data.conn_fd = sock_fd_own;
	
	int ret_code = -1;
	int addition_port = 0;

	while (ret_code < 0){
		addition_port++;

		con_data.client.sin_family = AF_INET; // IPv4
		con_data.client.sin_addr.s_addr = INADDR_ANY;
		con_data.client.sin_port = htons(OWN_PORT + addition_port);
	
		ret_code = bind(con_data.sock_fd, (const struct sockaddr *)&con_data.client, sizeof(con_data.client));
	}

	printf("Binded port %d\n", htons(OWN_PORT + addition_port));


	char user_name[MAX_COMMAND_LENGHT] = {};
	char ip_for_connection[MAX_COMMAND_LENGHT] = {};
	char port_for_connection[MAX_COMMAND_LENGHT] = {};

	// argv 3 _USER_@_IP_:_PORT_

	char * space_user = strchr(argv[3],'@');
	if (space_user == NULL){
		printf("Enter the correct input: _USER_@_IP_:_PORT_ (your input: %s)\n", argv[3]);
		exit(EXIT_FAILURE);
	}
	
	int len_user = space_user - argv[3];
	strncpy(user_name, argv[3], len_user);

	char * space_ip = strchr(argv[3], ':');
	if (space_user == NULL){
		printf("Enter the correct input: _USER_@_IP_:_PORT_ (your input: %s)\n", argv[3]);
		exit(EXIT_FAILURE);
	}
	int len_ip = space_ip - space_user - 1;
	strncpy(ip_for_connection, space_user + 1, len_ip);
	strcpy(port_for_connection, space_ip + 1);

	con_data.admin.sin_family = AF_INET;
	inet_pton(AF_INET, ip_for_connection, &con_data.admin.sin_addr);
	con_data.admin.sin_port = htons(atoi(port_for_connection));

	//printf("Admin %s:%s\n", ip_for_connection, port_for_connection);
	printf("Admin %s:%d\n", inet_ntoa(con_data.admin.sin_addr), htons(con_data.admin.sin_port));

	Generetion_keys(mode, &con_data);

	//printf("In data\n%s %s\n", con_data.info_user.key, con_data.info_user.IV);

	//printf("OWN ip = %s, port = %d\n", inet_ntoa(con_data.my.sin_addr), ntohs(con_data.my.sin_port));

	int code = 1;
	input enter;

	// do sender and reciever

	int type = fork();
	if (type){
		// reciever
		char * message = calloc(MAX_COMMAND_LENGHT, sizeof(char));

		while (1){
			memset(message, '\0', MAX_COMMAND_LENGHT);
			read(STDIN_FILENO, message, MAX_COMMAND_LENGHT);
			Send_message(mode, &con_data, message);
		}
	}else{
		// sendler 
		char * content = calloc(MAX_COMMAND_LENGHT, sizeof(char));

		while (1){
			memset(content, '\0', MAX_COMMAND_LENGHT);
			Get_message(mode, &con_data, content);
			write(STDOUT_FILENO, content, strlen(content));
		}
	}

	return 0;
}


