#include "../headers/client.h"

struct sockaddr_in Get_addr(){
	struct sockaddr_in own_addr = {};
	own_addr.sin_family = AF_INET;
	own_addr.sin_port = htons(OWN_PORT);
	own_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	return own_addr;
}

void Bind_addr(int sock_fd_own, struct sockaddr_in * own_addr){
	if ((bind(sock_fd_own, (struct sockaddr *)&own_addr, sizeof(own_addr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
}