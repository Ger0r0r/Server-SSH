#include "../headers/client.h"

int Connection_attempt(SSI own_addr, SSI * adm_addr){

	memset(adm_addr, 0, sizeof(*adm_addr));

	int ret = Broadcast_find(own_addr, adm_addr);

	if (ret){
		printf("Get admin %s:%d\n", inet_ntoa(adm_addr->sin_addr), ntohs(adm_addr->sin_port));
		return 1;
	}else{
		printf("\n\tConnection failed\n\n");
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

void Generetion_keys(connection * data){
	char * hey = "@Hey admin!\n";
	sendto(data->sock_fd, (const char *)hey, strlen(hey), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));

	size_t K1, K2;
	Preparing_numeral_keys(data->sock_fd, data->admin, &K1, &K2);
	char Key_new[MAX_COMMAND_LENGHT] = {0};
	char IV_new[MAX_COMMAND_LENGHT] = {0};
	Make_keys(K1, K2, Key_new, IV_new);
}

int Check_for_old_keys(connection * data){
	int secret_txt = open(".login.txt", O_CREAT, 0700);

	char buf[MAX_COMMAND_LENGHT] = {0};

	// in .login.txt "__LOGIN__#__PASSWORD__#__KEY__#__IV__"

	int count = read(secret_txt, buf, MAX_COMMAND_LENGHT);
	if (count < 0){
		perror("Read secret.txt");
	}else if (count > 32){
		char * get_password = strchr(buf, '#');
		char * get_key_old = strchr(get_password + 1, '#');
		char * get_IV_old = strchr(get_key_old + 1, '#');

		buf[get_password - buf] = '\0';
		buf[get_key_old - buf] = '\0';
		buf[get_IV_old - buf] = '\0';

		strcat(data->info_user.login, buf);
		strcat(data->info_user.password, get_password + 1);
		strcat(data->info_user.key_old, get_key_old + 1);
		strcat(data->info_user.IV_old, get_IV_old + 1);

		printf("Read:\n%s\n%s\n%s\n%s\n", data->info_user.login, data->info_user.password, data->info_user.key_old, data->info_user.IV_old);
		return 1;		

	}else{
		printf("Previos session keys does'n exists\n");
		close(secret_txt);
		return -1;		
	}
	return -1;
}

int Auto_login(connection * data){
	char another_ses[MAX_COMMAND_LENGHT] = {0};
	sprintf(another_ses, "@Have previos session:%s#%s#%s#%s\n", data->info_user.login, data->info_user.password, data->info_user.key_old, data->info_user.IV_old);
	/**/sendto(data->sock_fd, (const char *)another_ses, strlen(another_ses), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));

	int n;
	unsigned int len;
	char admin_message[MAX_COMMAND_LENGHT] = {0};
	/**/n = recvfrom(data->sock_fd, (char *)admin_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&data->admin, &len);
	admin_message[n] = '\0';

	if (strcmp(admin_message, "@Success")){
		data->status = 2;
		int secret_info = open(".login.txt", O_CREAT, O_TRUNC, 0700);

		dprintf(secret_info, "%s#%s#%s#%s\n", data->info_user.login, data->info_user.password, data->info_user.key, data->info_user.IV);

		close(secret_info);
		return 1;
	}else{
		return 2;
	}
	return -1;
}