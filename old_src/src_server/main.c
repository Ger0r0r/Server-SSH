#include "../headers/server.h"

int main(int argc, char ** argv) {

	//int ret = fork();
	//if (ret){
	//	while (1){}
	//}else{
	//	setsid();
	//	printf("Deamon:%d", getpid());
	//	kill(getppid(), SIGKILL);
	//}
	
	int mode = -1; // 0 - UDP; 1 - TCP; -1 - ERROR

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
	log_perror("strcmp in main");
	
	int code = fork();
	log_perror("fork in main");

	if (code){
		Broadcast_scanning();
	}else if (code == 0) {
		Wait_connection(mode);
	}else{
		log_error("fork return < 0");
		exit(EXIT_FAILURE);
	}

	return 0;
}