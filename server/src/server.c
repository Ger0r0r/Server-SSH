//#include "../headers/server.h"

//void XXXX() {
//	int sockfd;
//	char buffer[MAX_COMMAND_LENGHT];
//	char *hello = "Hello from server";
//	SSI servaddr, cliaddr;
	
//	// Creating socket file descriptor
//	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
//		perror("socket creation failed");
//		exit(EXIT_FAILURE);
//	}
	
//	memset(&servaddr, 0, sizeof(servaddr));
//	memset(&cliaddr, 0, sizeof(cliaddr));
	
//	// Filling server information
//	servaddr.sin_family = AF_INET; // IPv4
//	servaddr.sin_addr.s_addr = INADDR_ANY;
//	servaddr.sin_port = htons(BROADCAST_PORT);
	
//	// Bind the socket with the server address
//	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
//			sizeof(servaddr)) < 0 )
//	{
//		perror("bind failed");
//		exit(EXIT_FAILURE);
//	}
	
//	int len, n;

//	len = sizeof(cliaddr); //len is value/result

//	n = recvfrom(sockfd, (char *)buffer, MAX_COMMAND_LENGHT,
//				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
//				&len);
//	buffer[n] = '\0';
//	printf("Client : %s\n", buffer);
//	sendto(sockfd, (const char *)hello, strlen(hello),
//		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
//			len);
//	printf("Hello message sent.\n");
	
//	return;
//}
