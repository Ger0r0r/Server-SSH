#include "../headers/client.h"

int main() {

	int code = 1;
	input enter;
	connection server_info; 

	while (code){
		printf("> ");
		enter = Read_input();
		code = Get_code(enter);
		//Do_task(enter, server_info,code);
	}

	return 0;
}