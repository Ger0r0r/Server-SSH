#include "../headers/server.h"

SSI adm_addr, cli_addr;

void Administraitor_UDP(){
	
	int sock_fd_adm;
	char buf[MAX_COMMAND_LENGHT];
	
	// Creating socket file descriptor
	if ( (sock_fd_adm = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&adm_addr, 0, sizeof(adm_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));
	
	// Filling server information
	adm_addr.sin_family = AF_INET; // IPv4
	adm_addr.sin_addr.s_addr = INADDR_ANY;
	adm_addr.sin_port = htons(BROADCAST_PORT);
	
	// Bind the socket with the server address

	int ret_code = -1;
	int addition_port = 0;

	while (ret_code < 0){
		addition_port++;

		adm_addr.sin_family = AF_INET; // IPv4
		adm_addr.sin_addr.s_addr = INADDR_ANY;
		adm_addr.sin_port = htons(BROADCAST_PORT + addition_port);
	
		ret_code = bind(sock_fd_adm, (const struct sockaddr *)&adm_addr, sizeof(adm_addr));
	}

	size_t mine_info = Encrypt_signal(adm_addr);

	union sigval cn_info;
	memset(&cn_info, 0, sizeof(cn_info));
	cn_info.sival_ptr = (void*)mine_info;
	sigqueue(getppid(), SIGUSR1, cn_info);
	
	//int len, n;

	//len = sizeof(cli_addr); //len is value/result

	//n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cli_addr, &len);
	//buffer[n] = '\0';
	//printf("Client : %s\n", buffer);
	//sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &cli_addr, len);
	//printf("Hello message sent.\n");
	
	return;


}

void Administraitor_TCP(SSI client, int ccl){
	return;
}