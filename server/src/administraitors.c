#include "../headers/server.h"

SSI admin_addr, client_addr;

void Administraitor(int mode){

	char buf[MAX_COMMAND_LENGHT];

	connection * bfd = calloc(1, sizeof(connection));

	if (mode){ // UDP = 0 TCP = 1
		if ((bfd->sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("socket creation failed...\n");
			exit(0);
		}
	}else{
		if ((bfd->sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
			perror("socket creation failed");
			exit(EXIT_FAILURE);
		}
	}

	memset(&bfd->admin, 0, sizeof(bfd->admin));
	log_perror("memset 1 in Administrator");
	memset(&bfd->client, 0, sizeof(bfd->client));
	log_perror("memset 2 in Administrator");
	
	// Filling server information
	bfd->admin.sin_family = AF_INET; // IPv4
	bfd->admin.sin_addr.s_addr = INADDR_ANY;
	bfd->admin.sin_port = htons(BROADCAST_PORT);
	
	// Bind the socket with the server address
	int ret_code = -1;
	int addition_port = 0;

	while (ret_code < 0){
		addition_port++;

		bfd->admin.sin_family = AF_INET; // IPv4
		bfd->admin.sin_addr.s_addr = INADDR_ANY;
		bfd->admin.sin_port = htons(BROADCAST_PORT + addition_port);
	
		ret_code = bind(bfd->sock_fd, (const struct sockaddr *)&bfd->admin, sizeof(bfd->admin));
		log_perror("bind in Administrator");
	}


	// Give info to broadcast handler
	size_t mine_info = Encrypt_signal(bfd->admin);
	union sigval cn_info;
	memset(&cn_info, 0, sizeof(cn_info));
	log_perror("memset 3 in Administrator");
	cn_info.sival_ptr = (void*)mine_info;
	sigqueue(getppid(), SIGUSR1, cn_info);
	
	// Waiting for connection
	if (mode){
		if ((listen(bfd->sock_fd, 5)) != 0) {
			printf("Listen failed...\n");
			exit(0);
		}

		// Accept the data packet from client and verification
		int len = (sizeof(bfd->client));
		if ((bfd->conn_fd = accept(bfd->sock_fd, (SA*)&bfd->client, &len)) < 0) {
			printf("server accept failed...\n");
			exit(0);
		}
	}else{
		int n;
		socklen_t len = sizeof(bfd->client);
		memset(buf, 0, sizeof(buf));
		log_perror("memset 4 in Administrator");
		n = recvfrom(bfd->sock_fd, (char *)buf, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&bfd->client, &len);
		log_perror("recvfrom in Administrator");
	}
	// Message: @Hey admin!

	// Give signal for creating new admin to connection handler
	kill(getppid(), SIGUSR2);
	log_perror("kill in Administrator");

	// Generate two secret keys
	size_t K1, K2;
	Preparing_numeral_keys(mode, bfd, &K1, &K2);
	char * Key = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	char * IV = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	Make_keys(K1, K2, Key, IV);
	bfd->key = Key;
	bfd->iv = IV;

	int code = 1;
	char message[MAX_COMMAND_LENGHT] = {0};
	char content[MAX_COMMAND_LENGHT] = {0};

	Get_message(mode, bfd, message);
	if (User_login(message))
		log_error_handler("user_login");
	
	


	while (code){
		Wait_message(bfd, message);
		code = Parser(message, content, bfd);
	}
	exit(EXIT_SUCCESS);
}