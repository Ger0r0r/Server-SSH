#include "../headers/server.h"

int main() {

	int code = fork();

	if (code){
		Wait_connection();
	}else{
		Broadcast_scanning();
	}

	return 0;
}