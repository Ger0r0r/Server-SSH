#include "../headers/client.h"

int mode;

int main(int argc, char ** argv) {

	if (argc != 2){
		printf("Start program with mode: %s -udp/-tcp\n", argv[0]);
		return 0;
	}
	if (strcmp("-udp", argv[1]) == 0){
		mode = 0;
	}else if (strcmp("-tcp", argv[1]) == 0){
		mode = 1;
	}else{
		printf("Enter correct mode: -udp/-tcp (you input: %s)\n", argv[1]);
		return 0;
	}

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