#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE_OF_BUFFER 2000

int main (int argc, char *argv[]) {

	char * buffer = calloc (SIZE_OF_BUFFER, sizeof(char));

	if(argc != 2) {
		printf("\n Usage: %s <UDP - 1, TCP - 2> \n",argv[0]);
		return 1;
	}

	int type = atoi (argv[1]);

	switch (type){
	case 1:{

		// UDP

		struct sockaddr_in sa;

		char * buffer = calloc (SIZE_OF_BUFFER, sizeof(char));
		gets(buffer);
		int buffer_length = strlen(buffer) + 1;

		int sock = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );

		if (sock == -1){
			perror("SOCKET");
			return 0;
		}

		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = htonl(0x7F000001);
		sa.sin_port = htons(1100);

		int bytes_sent = sendto(sock, buffer, buffer_length, 0, (struct sockaddr*)&sa, sizeof(struct sockaddr_in));

		if (bytes_sent < 0)
			perror("SENDTO");

		close(sock);
		return 0;
		break;
	}
	case 2:{

		// TCP

		struct sockaddr_in stSockAddr;
		int i32SocketFD = socket(PF_INET, SOCK_STREAM, 0);

		if (i32SocketFD == -1) {
			perror("Error Socket");
			return EXIT_FAILURE;
		}

		memset(&stSockAddr, 0, sizeof (stSockAddr));

		stSockAddr.sin_family = AF_INET;
		stSockAddr.sin_port = htons(1100);
		int i32Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);

		if (i32Res < 0) {
			perror("Error Address");
			close(i32SocketFD);
			return EXIT_FAILURE;
		} else if (!i32Res) {
			perror("Error IP");
			close(i32SocketFD);
			return EXIT_FAILURE;
		}

		if (connect(i32SocketFD, (struct sockaddr*) &stSockAddr, sizeof (stSockAddr)) == -1) {
			perror("Error Connect");
			close(i32SocketFD);
			return EXIT_FAILURE;
		}
		gets(buffer);
		send(i32SocketFD, buffer, strlen(buffer), 0);

		free(buffer);
		shutdown(i32SocketFD, SHUT_RDWR);
		close(i32SocketFD);
		return 0;
		break;
	}
	default:
		printf("\n Usage: %s <UDP - 1, TCP - 2> \n",argv[0]);
		return 1;
		break;
	}
}