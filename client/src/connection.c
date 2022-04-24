#include "../headers/client.h"

int Connection_attempt(){

	int ret = Broadcast_find();

	if (ret){
		
	}else{
		printf("\tConnection failed\n");
	}
	return 0;
}


int Broadcast_find(){
	char buf[MAX_COMMAND_LENGHT] = "@Give_info";

	struct sockaddr_in serv_addr;
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

	int n;

	for (size_t i = 0; i < COUNT_OF_ATTEMPT_FOR_CONNECTION; i++){
		printf("\tAttempt %zu\n", i + 1);

		sendto(sock_fd, buf, strlen(buf), MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof serv_addr);

		// get
		socklen_t len = sizeof(serv_addr);
		n = recvfrom(sock_fd, buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr *) &serv_addr, &len);
		
		printf("\tGet: %s\n", buf);

		if (n && (strncmp(buf, "@Wait for administraitor\n", 18) == 0)){
			printf("\tGet answer from server: ip = %s\n\tMessage : %s\n", inet_ntoa(serv_addr.sin_addr), buf);
			return 1;
		}
		usleep(TIME_BETWEEN_ATTEMPTS_FOR_CONNECTION);
	}
	return 0;
}