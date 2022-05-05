#include "../headers/client.h"

int mode;

int main(int argc, char ** argv) {
	// ./_PROG_NAME_ -t TCP/UDP _USER_@_IP_
	// ./_PROG_NAME_ -broadcast


	if (argc != 2){
		printf("Start program with task:\n%s -t (connect)\n%s -broadcast (find servers)\n", argv[0], argv[0]);
		return 0;
	}

    struct sockaddr_in server;
    int socket_ss = 0;
	
	char NEED_BIND = 1;
	char NOT_NEED_BIND = 2;

	if (strcmp("-broadcast", argv[1]) == 0){
		printf("CHECK\n");
		socket_ss = Socket_config(&server, BROADCAST_PORT, SOCK_DGRAM, SO_BROADCAST, NOT_NEED_BIND, htonl(INADDR_BROADCAST));
        if (Broadcast_search(socket_ss, &server) == 0)
            return 0;
	}else if (strcmp("-t", argv[1])){
		if (strcmp("TCP", argv[2]) == 0){
			mode = 0;
		}else if (strcmp("UDP", argv[2]) == 0){
			mode = 1;
		}else{
			printf("Enter correct mode: TCP or UDP\ncorrect input: %s -t <TCP/UDP> <USER>@<IP>\nyou input: %s -t %s\n", argv[0], argv[0], argv[2]);
			return 0;
		}

		// connect

	}
	return 0;




	int sock_fd_own = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd_own == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}

	connection con_data;
	memset(&con_data, 0, sizeof(con_data));
	con_data.my.sin_family = AF_INET;
	con_data.my.sin_port = htons(OWN_PORT);
	con_data.my.sin_addr.s_addr = htonl(INADDR_ANY);
	con_data.status = 0;
	con_data.sock_fd = sock_fd_own;
	
	if ( bind(sock_fd_own, (struct sockaddr *)&con_data.my, sizeof(con_data.my)) < 0 ){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

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