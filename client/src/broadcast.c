#include "../headers/client.h"

int Broadcast_find(){
	char buf[MAX_COMMAND_LENGHT] = {0};

	sprintf(buf, "Hello!\n");

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

	int time_start = time(NULL);

	int a = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
	//bind(sock_fd, (struct sockaddr*) &serv_addr, sizeof (serv_addr));

	//printf("BR ip = %s, port = %d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));

	int n;
	sendto(sock_fd, buf, strlen(buf), MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof serv_addr);

	SSI previos;
	memcpy(&previos, &serv_addr, sizeof(serv_addr));

	while (time(NULL) - time_start < 3){
		//printf("\tAttempt %zu ", i + 1);

		socklen_t len = sizeof(serv_addr);
		n = recvfrom(sock_fd, buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr *) &serv_addr, &len);
		
		

		//printf("\tGet: %s\n", buf);

		if (n && (previos.sin_addr.s_addr != serv_addr.sin_addr.s_addr)){
			printf("\tGet answer from server: ip = %s\n", inet_ntoa(serv_addr.sin_addr));
			previos.sin_addr.s_addr = serv_addr.sin_addr.s_addr;
		}
	}
	return 0;
}