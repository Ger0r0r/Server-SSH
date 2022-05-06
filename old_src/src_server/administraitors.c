#include "../headers/server.h"

SSI admin_addr, client_addr;

void Administraitor_UDP(){

	char buf[MAX_COMMAND_LENGHT];

	connection * bfd = calloc(1, sizeof(connection));
	
	// Creating socket file descriptor;

	if ((bfd->sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		log_perror("socket in Administrator UDP");
		exit(EXIT_FAILURE);
	}
	

	memset(&bfd->admin, 0, sizeof(bfd->admin));
	log_perror("memset 1 in Administrator UDP");
	memset(&bfd->client, 0, sizeof(bfd->client));
	log_perror("memset 2 in Administrator UDP");
	
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
		log_perror("bind in Administrator UDP");
	}

	// Give info to broadcast handler
	size_t mine_info = Encrypt_signal(bfd->admin);
	union sigval cn_info;
	memset(&cn_info, 0, sizeof(cn_info));
	log_perror("memset 3 in Administrator UDP");
	cn_info.sival_ptr = (void*)mine_info;
	sigqueue(getppid(), SIGUSR1, cn_info);
	
	// Waiting for connection
	int n;
	socklen_t len = sizeof(bfd->client);
	memset(buf, 0, sizeof(buf));
	log_perror("memset 4 in Administrator UDP");
	n = recvfrom(bfd->sock_fd, (char *)buf, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&bfd->client, &len);
	log_perror("recvfrom in Administrator UDP");
	// Message: @Hey admin!
	// Give signal for creating new admin to connection handler
	kill(getppid(), SIGUSR2);
	log_perror("kill in Administrator UDP");

	// Generate two secret keys
	size_t K1, K2;
	Preparing_numeral_keys(bfd->sock_fd, bfd->client, &K1, &K2);
	char * Key = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	char * IV = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	Make_keys(K1, K2, Key, IV);
	bfd->key = Key;
	bfd->iv = IV;

	int code = 1;
	char message[MAX_COMMAND_LENGHT] = {0};
	char content[MAX_COMMAND_LENGHT] = {0};

	//printf("Info about connection:\n");
	//printf("Discriptor - %d\n", bfd->sock_fd);
	//printf("Client - %s:%d\n", inet_ntoa(bfd->client.sin_addr), htons(bfd->client.sin_port));
	//printf("Keys - %s %s\n", bfd->key, bfd->iv);
	//printf("Database\n");
	//for (size_t i = 0; i < bfd->c_users; i++){
	//	printf("%s %s %s %s\n", bfd->users[i]->login, bfd->users[i]->password, bfd->users[i]->key_old, bfd->users[i]->IV_old);
	//}printf("\n");
	
	while (code){
		//printf("Wait message\n");
		Get_message(bfd, message);
		//printf("Well well,.. I have\n%s\n", message);
		code = Parser(message, content, bfd);
	}
	exit(EXIT_SUCCESS);
}

void Administraitor_TCP(connection * bfd){
	return;
}