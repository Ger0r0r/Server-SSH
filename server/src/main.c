#include "../headers/server.h"

int main(int argc, char ** argv) {

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
	
	int code = fork();

	if (code){
		Broadcast_scanning();
	}else{
		Wait_connection(mode);
	}

	return 0;
}