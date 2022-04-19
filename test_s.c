#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE_OF_BUFFER 2000

#define port 1100

int main (int argc, char *argv[]) {
	if(argc != 2) {
		printf("\n Usage: %s <UDP - 1, TCP - 2> \n",argv[0]);
		return 1;
	}


	char * buffer = calloc (SIZE_OF_BUFFER, sizeof(char));

	int type = atoi (argv[1]);

	switch (type){
	case 1:{

		// UDP

		int sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

		struct sockaddr_in sa;
		int bound;
		ssize_t recsize;
		socklen_t *address_len = NULL;

		sa.sin_addr.s_addr = htonl(INADDR_ANY);
		sa.sin_port = htons(port);

		bound = bind( sock, ( struct sockaddr* )&sa, sizeof( struct sockaddr ) );

		if ( bound < 0 )
			perror("BIND");

		while(1){
			//printf("recv test....\n");
			recsize = recvfrom(sock, (void*)buffer, 100, 0, (struct sockaddr*)&sa, address_len );

			//if (recsize < 0)
			//	perror("RECVFROM");

			//sleep( 1 );
			printf("%s\n", buffer);
		}


		break;
	}
	case 2:{

		// TCP

		struct sockaddr_in stSockAddr;
		int i32SocketFD = socket(AF_INET, SOCK_STREAM, 0);

		if (i32SocketFD == -1) {
			perror("Error: made socket");
			exit(EXIT_FAILURE);
		}

		memset(&stSockAddr, 0, sizeof (stSockAddr));

		stSockAddr.sin_family = AF_INET;
		stSockAddr.sin_port = htons(port);
		stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(i32SocketFD, (struct sockaddr*) &stSockAddr, sizeof (stSockAddr)) == -1) {
			perror("Error Bind");

			close(i32SocketFD);
			exit(EXIT_FAILURE);
		}

		if (listen(i32SocketFD, 10) == -1) {
			perror("Error Listen");

			close(i32SocketFD);
			exit(EXIT_FAILURE);
		}

		for (;;) {
			int i32ConnectFD = accept(i32SocketFD, 0, 0);

			if (i32ConnectFD < 0) {
				perror("Error accept");
				close(i32SocketFD);
				exit(EXIT_FAILURE);
			}

			read(i32ConnectFD, buffer, SIZE_OF_BUFFER);
			printf("%s\n", buffer);

			free(buffer);
			char * buffer = calloc (SIZE_OF_BUFFER, sizeof(char));
			shutdown(i32ConnectFD, SHUT_RDWR);
			close(i32ConnectFD);
		}

		return 0;

		break;
	}
	default:
		printf("\n Usage: %s <UDP - 1, TCP - 2> \n",argv[0]);
		return 1;
		break;
	}	
}