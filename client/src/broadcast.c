#include "../headers/client.h"

 int Broadcast_search(int socket, struct sockaddr_in* server) {
	const char SEARCH = '2';
	if (sendto(socket, &SEARCH, sizeof(char), 0, (struct sockaddr*) server, sizeof(*server)) < 0) {
		//log_perror("sendto");
		exit(EXIT_FAILURE);
	}
	int button = 0;
	int time_start = time(NULL);
	SSI tmp_addr;
	memset(&tmp_addr, 0, sizeof(tmp_addr));
	socklen_t tmp_addr_len = sizeof(tmp_addr);
	struct pollfd fd_in[1];
	fd_in[0].fd = socket;
	fd_in[0].events = POLLIN;
	while (time(NULL) - time_start < 10) {
		int ret = poll(fd_in, 2, SEARCHING_SERVERS_TIME);
		if (ret == -1) {
			//log_perror("poll\n");
			exit(EXIT_FAILURE);
		}
		else if (ret == 0)
			break;
		int port;
		if (recvfrom(socket, &port, sizeof(int), 0, (struct sockadd *)&tmp_addr, &tmp_addr_len) > 0) 
			printf("The server %s was found\n", inet_ntoa(tmp_addr.sin_addr));
		else {
			//log_perror("recvfrom\n");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
 }

int Broadcast_find(SSI own_addr, SSI * ret_addr){
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

		if (n && (strncmp(buf, "@Your administraitor:\n", 20) == 0)){
			printf("\tGet answer from server: ip = %s\n\tMessage : %s\n", inet_ntoa(serv_addr.sin_addr), buf);

			char * point = strchr(buf, ':');
			point++;

			printf("\nPick up port: %s\n", point);

			ret_addr->sin_family = AF_INET;
			ret_addr->sin_addr = serv_addr.sin_addr;
			ret_addr->sin_port = htons(atoi(point));

			return 1;
		}
	}
	return 0;
}