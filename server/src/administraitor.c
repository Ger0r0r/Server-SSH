#include "../headers/server.h"

SSI admin_addr, client_addr;

void Administraitor_UDP(){
	
	int sock_fd_adm;
	char buf[MAX_COMMAND_LENGHT];
	
	// Creating socket file descriptor
	if ( (sock_fd_adm = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&admin_addr, 0, sizeof(admin_addr));
	memset(&client_addr, 0, sizeof(client_addr));
	
	// Filling server information
	admin_addr.sin_family = AF_INET; // IPv4
	admin_addr.sin_addr.s_addr = INADDR_ANY;
	admin_addr.sin_port = htons(BROADCAST_PORT);
	
	// Bind the socket with the server address
	int ret_code = -1;
	int addition_port = 0;

	while (ret_code < 0){
		addition_port++;

		admin_addr.sin_family = AF_INET; // IPv4
		admin_addr.sin_addr.s_addr = INADDR_ANY;
		admin_addr.sin_port = htons(BROADCAST_PORT + addition_port);
	
		ret_code = bind(sock_fd_adm, (const struct sockaddr *)&admin_addr, sizeof(admin_addr));
	}

	size_t mine_info = Encrypt_signal(admin_addr);

	union sigval cn_info;
	memset(&cn_info, 0, sizeof(cn_info));
	cn_info.sival_ptr = (void*)mine_info;
	sigqueue(getppid(), SIGUSR1, cn_info);
	
	
	int n;
	socklen_t len = sizeof(client_addr);
	memset(buf, 0, sizeof(buf));
	n = recvfrom(sock_fd_adm, (char *)buf, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&client_addr, &len);
	// Message: @Hey admin!

	kill(getppid(), SIGUSR2);

	size_t K1, K2;
	Preparing_numeral_keys(sock_fd_adm, client_addr, &K1, &K2);
	char Key[MAX_COMMAND_LENGHT] = {0};
	char IV[MAX_COMMAND_LENGHT] = {0};
	Make_keys(K1, K2, Key, IV);

	return;


}

void Administraitor_TCP(SSI client, int ccl){
	return;
}