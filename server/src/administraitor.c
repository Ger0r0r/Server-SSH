#include "../headers/server.h"

void Administraitor_UDP(SSI client, int ccl){

	char buffer[MAX_COMMAND_LENGHT] = {0};

	int sock_fd_adm;

	// Creating socket file descriptor
	if ( (sock_fd_adm = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	SSI admin_addr;
	memset(&admin_addr, 0, sizeof(admin_addr));
	admin_addr.sin_family = AF_INET;
	admin_addr.sin_port = htons(BROADCAST_PORT + ccl);
	admin_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ( bind(sock_fd_adm, (const struct sockaddr *)&admin_addr, sizeof(admin_addr)) < 0 ){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	SSI client_addr;
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = client.sin_port;
	client_addr.sin_addr = client.sin_addr;

	Preparing_keys(sock_fd_adm, client_addr);

	while (1){
		
	}
	
}

void Administraitor_TCP(SSI client, int ccl){
	return;
}