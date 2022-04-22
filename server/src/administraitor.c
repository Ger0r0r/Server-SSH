#include "../headers/server.h"

void Administraitor_UDP(connection client){

	char buffer[MAX_COMMAND_LENGHT] = {0};
	int sock_fd_adm;

	// Creating socket file descriptor
	if ( (sock_fd_adm = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = client.port;
	client_addr.sin_addr = client.addr;

	sendto();




	while (1){
		
	}
	
}
