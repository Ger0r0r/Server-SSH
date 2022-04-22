#include "../headers/server.h"

int current_client = 0;
int mode = -1; // 0 - UDP; 1 - TCP; -1 - ERROR
int public_dh_key_p = 9887;
int public_dh_key_g = 7;

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
	
	int code = fork();

	if (code){
		Wait_connection();
	}else{
		Broadcast_scanning();
	}

	return 0;
}