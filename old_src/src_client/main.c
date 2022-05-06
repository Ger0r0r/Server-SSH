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
		if (Broadcast_search(socket_ss, &server) == 0)
			return 0;
	}else if (strcmp("-t", argv[1]) == 0){
		if (strcmp("TCP", argv[2]) == 0){
			mode = 0;
		}else if (strcmp("UDP", argv[2]) == 0){
			mode = 1;
		}else{
			printf("Enter correct mode: TCP or UDP\ncorrect input: %s -t <TCP/UDP> <USER>@<IP:PORT>\nyou input: %s -t %s\n", argv[0], argv[0], argv[2]);
			return 0;
		}
	}else{
		printf("Start program with task:\n%s -t (connect)\n%s -broadcast (find servers)\n", argv[0], argv[0]);
		return 0;
	}


	int sock_fd_own = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd_own == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}

	connection con_data;
	memset(&con_data, 0, sizeof(con_data));
	con_data.status = 0;
	con_data.sock_fd = sock_fd_own;
	
	int ret_code = -1;
	int addition_port = 0;

	while (ret_code < 0){
		addition_port++;

		con_data.my.sin_family = AF_INET; // IPv4
		con_data.my.sin_addr.s_addr = INADDR_ANY;
		con_data.my.sin_port = htons(OWN_PORT + addition_port);
	
		ret_code = bind(con_data.sock_fd, (const struct sockaddr *)&con_data.my, sizeof(con_data.my));
	}

	printf("Binded port %d\n", htons(OWN_PORT + addition_port));

	char user_name[MAX_COMMAND_LENGHT] = {};
	char ip_for_connection[MAX_COMMAND_LENGHT] = {};
	char port_for_connection[MAX_COMMAND_LENGHT] = {};

	// argv 3 _USER_@_IP_:_PORT_

	char * space_user = strchr(argv[3],'@');
	int len_user = space_user - argv[3];
	strncpy(user_name, argv[3], len_user);

	char * space_ip = strchr(argv[3], ':');
	int len_ip = space_ip - space_user + 1;
	strncpy(ip_for_connection, space_user + 1, len_ip);
	strcpy(port_for_connection, space_ip + 1);

	con_data.admin.sin_family = AF_INET;
	inet_aton(ip_for_connection, &con_data.admin.sin_addr);
	con_data.admin.sin_port = htons(atoi(port_for_connection));

	if (mode = 0){
		
	}

	Generetion_keys(&con_data);

	//printf("In data\n%s %s\n", con_data.info_user.key, con_data.info_user.IV);

	//printf("OWN ip = %s, port = %d\n", inet_ntoa(con_data.my.sin_addr), ntohs(con_data.my.sin_port));

	int code = 1;
	input enter;

	while (code){
		printf("> ");
		enter = Read_input();
		code = Do_task(&con_data, enter);
	}
	return 0;
}


