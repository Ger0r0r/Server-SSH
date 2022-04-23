#include "../headers/client.h"

struct sockaddr_in Connection_attempt(){

	struct sockaddr_in serv_addr = Broadcast_find();

	return serv_addr;
}


struct sockaddr_in Broadcast_find(){
	char * buf = "@Give_info";
	read(STDIN_FILENO, buf, MAX_COMMAND_LENGHT);
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	int sock_fd;
	if ( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(27312);
	serv_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	int a = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
	//bind(sock_fd, (struct sockaddr*) &serv_addr, sizeof (serv_addr));

	sendto(sock_fd, buf, strlen(buf), MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof serv_addr);

	// get
	socklen_t len = sizeof(serv_addr);
	recvfrom(sock_fd, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *) &serv_addr, &len);
	printf("Get answer from server: ip = %s\nMessage : %s\n", inet_ntoa(serv_addr.sin_addr), buf);
	return serv_addr;
}