#include "../headers/client.h"

int Connection_attempt(SSI own_addr){

	int ret = Broadcast_find(own_addr);

	if (ret){
		
	}else{
		printf("\n\tConnection failed\n\n");
	}
	return 0;
}


int Broadcast_find(SSI own_addr){
	char buf[MAX_COMMAND_LENGHT] = {0};

	sprintf(buf, "@Hello! port:%d", htons(own_addr.sin_port));


	SSI serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	int sock_fd;
	if ( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(BROADCAST_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	int a = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
	//bind(sock_fd, (struct sockaddr*) &serv_addr, sizeof (serv_addr));

	//printf("BR ip = %s, port = %d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));

	int n;

	for (size_t i = 0; i < COUNT_OF_ATTEMPT_FOR_CONNECTION; i++){
		printf("\tAttempt %zu\n", i + 1);

		sendto(sock_fd, buf, strlen(buf), MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof serv_addr);

		usleep(TIME_BETWEEN_ATTEMPTS_FOR_CONNECTION);
		// get
		socklen_t len = sizeof(serv_addr);
		n = recvfrom(sock_fd, buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr *) &serv_addr, &len);
		
		//printf("\tGet: %s\n", buf);

		if (n && (strncmp(buf, "@Wait for administraitor\n", 18) == 0)){
			printf("\tGet answer from server: ip = %s\n\tMessage : %s\n", inet_ntoa(serv_addr.sin_addr), buf);
			return 1;
		}
	}
	return 0;
}