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

	int sock_fd_own = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd_own == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}

	struct sockaddr_in own_addr = Get_addr();
	Bind_addr(sock_fd_own, &own_addr);

	int code = 1;
	input enter;

	while (code){
		printf("> ");
		enter = Read_input();
		code = Do_task(enter);
	}

	return 0;
}