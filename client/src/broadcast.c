#include "../headers/client.h"

int Broadcast_search(int socket_ss, struct sockaddr_in* server) {
	const char SEARCH = '2';
	CHECK(sendto(socket_ss, &SEARCH, sizeof(char), 0, (struct sockaddr*) server, sizeof(*server)))
	
	int button = 0;
	int time_start = time(NULL);
	SSI tmp_addr;
	memset(&tmp_addr, 0, sizeof(tmp_addr));

	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = htons(BROADCAST_PORT);
	tmp_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	socklen_t tmp_addr_len = sizeof(tmp_addr);
	struct pollfd fd_in[1];
	fd_in[0].fd = socket_ss;
	fd_in[0].events = POLLIN;
	while (time(NULL) - time_start < 3) {
		int ret = poll(fd_in, 2, SEARCHING_SERVERS_TIME);
		if (ret == -1) {
			//log_perror("poll\n");
			exit(EXIT_FAILURE);
		}
		else if (ret == 0)
			break;
		char port[MAX_COMMAND_LENGHT] = {};
		CHECK(recvfrom(socket_ss, &port, sizeof(port), MSG_DONTWAIT, (struct sockadd *) &tmp_addr, &tmp_addr_len) > 0)
		int num_port = atoi(port);
		printf("Server found IP %s port %d\n", inet_ntoa(tmp_addr.sin_addr), num_port);
	}
	return 0;
}

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

	int a = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
	//bind(sock_fd, (struct sockaddr*) &serv_addr, sizeof (serv_addr));

	//printf("BR ip = %s, port = %d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));

	int n;
	sendto(sock_fd, buf, strlen(buf), MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof serv_addr);

	SSI previos;
	memcpy(&previos, &serv_addr, sizeof(serv_addr));

	for (size_t i = 0; i < 1000; i++){
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