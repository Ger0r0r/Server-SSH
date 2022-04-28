#include "../headers/server.h"

int Disconnected(connection * bfd){

	int file = open(".data.txt", O_CREAT | O_TRUNC | O_RDWR, 0700);

	printf("Write down new data\n");

	for (size_t i = 0; i < bfd->c_users; i++){
		printf("%s#%s#%s#%s\n", bfd->users[i]->login, bfd->users[i]->password, bfd->users[i]->key, bfd->users[i]->IV);
		dprintf(file, "%s#%s#%s#%s\n", bfd->users[i]->login, bfd->users[i]->password, bfd->users[i]->key, bfd->users[i]->IV);
	}
	
	close(file);

	return 0;
}

int Login(connection * bfd, char * log_pas){
	// log_pas "__LOGIN__#__PASSWORD__"

	char username[MAX_COMMAND_LENGHT] = {0};
	char password[MAX_COMMAND_LENGHT] = {0};

	char * border = strchr(log_pas, '#');
	border[0] = '\0';
	strcpy(username, log_pas);
	strcpy(password, border + 1);

	for (size_t i = 0; i < bfd->c_users; i++){
		if ((strcmp(bfd->users[i]->login, username) == 0) && (strcmp(bfd->users[i]->password, password) == 0)){
			char message[MAX_COMMAND_LENGHT] = "@Success";
			/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));
			printf("In bfd %s %s\n", bfd->key, bfd->iv);
			bfd->users[i]->IV = bfd->iv;
			bfd->users[i]->key = bfd->key;
			printf("In bfd %s %s\n", bfd->users[i]->key, bfd->users[i]->IV);
			return 1;
		}
	}
	printf("Wrong login or password!\n");
	char message[MAX_COMMAND_LENGHT] = "@Wrong username or password!";
	/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));
	return 1;	
}

int Check_previos_session(connection * bfd, char * data){
	// @Have_previos_session __LOGIN__#__PASSWORD__#__OLD_KEY__#__OLD_IV__

	char * border = strchr(data, '#');
	char * border_start = strchr(border + 1, '#');
	char * border_between = strchr(border_start + 1, '#');
	border[0] = '\0';
	border_start[0] = '\0';
	border_between[0] = '\0';

	char old_key[MAX_COMMAND_LENGHT] = {0};
	char old_iv[MAX_COMMAND_LENGHT] = {0};

	strcpy(old_key, border_start+1);
	strcpy(old_iv, border_between+1);

	for (size_t i = 0; i < bfd->c_users; i++){
		printf("Compare\n%s and %s\n\%s and %s\n", bfd->users[i]->key_old, old_key, bfd->users[i]->IV_old, old_iv);
		printf("results %d %d\n",strcmp(bfd->users[i]->key_old, old_key), strcmp(bfd->users[i]->IV_old, old_iv));
		if ((strcmp(bfd->users[i]->key_old, old_key) == 0) && (strcmp(bfd->users[i]->IV_old, old_iv) == 0)){
			printf("Matched login and password!!!\n");
			char message[MAX_COMMAND_LENGHT] = "@Success";
			/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));
			printf("In bfd %s %s\n", bfd->key, bfd->iv);
			bfd->users[i]->IV = bfd->iv;
			bfd->users[i]->key = bfd->key;
			printf("In bfd %s %s\n", bfd->users[i]->key, bfd->users[i]->IV);
			return 1;
		}
	}
	char message[MAX_COMMAND_LENGHT] = "@Wrong";
	/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));
	return 1;	
}

int Do_usual(connection * bfd, char * command){

	int stat;
	int file_pipes[2];
	if	(pipe(file_pipes) == -1){
		perror("PIPE");
	}
	
	char ** argv = calloc(MAX_USER_COUNT, sizeof(char *));
	char * space = command;
	argv[0] = command;
	int count = 1;
	printf("\n\nHAH\n\n");

	while (1){
		space = strchr(space + 1, ' ');
		if (space == NULL){
			break;
		}
		space[0] = '\0';
		argv[count] = space + 1;
		count++;
		argv[count] = NULL;
	}

	printf("\n\nHAH\n\n");

	printf("Check char **\n");
	for (size_t i = 0; i < count; i++){
		printf("%p - %s\n", argv[i], argv[i]);
	}printf("%p\n", argv[count + 1]);
	
	printf("\n\nHAH\n\n");

	char * output = calloc(MAX_OUTPUT_LENGHT, sizeof(char));
	int read_c;
	int code = fork();
	if (!code){
		close(file_pipes[0]);
		dup2(file_pipes[1], STDOUT_FILENO);
		dup2(file_pipes[1], STDERR_FILENO);
		execvp(argv[0],  argv);
		printf("AHAHAH HUESOS\n");
		return -9;
	}else{
		close(file_pipes[1]);
		wait(&stat);
		read_c = read(file_pipes[0], output, MAX_OUTPUT_LENGHT);
		printf("Read already %d\n", read_c);
		perror("READ PIPE");
	}

	printf("\n\nHAH\n\n");

	/**/sendto(bfd->sock_fd, output, read_c, MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));
	return 1;
}

int Copy_to(connection * bfd, char * message){
	// __TARGET_PATH__
	// __SIZE__
	// __TEXT__

	

	int fd_file = open(message, O_CREAT | O_TRUNC | O_RDWR, 0777);

	char size_str[MAX_COMMAND_LENGHT] = {0};

	socklen_t len = sizeof(bfd->client);
	recvfrom(bfd->sock_fd, size_str, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&bfd->client, &len);
	int size = atoi(size_str);

	char * text = calloc(size, sizeof(char));
	recvfrom(bfd->sock_fd, text, size, MSG_WAITALL, (struct sockaddr *)&bfd->client, &len);

	write(fd_file, text, size);

	char answer[MAX_COMMAND_LENGHT] = "@Success";
	/**/sendto(bfd->sock_fd, answer, strlen(answer), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));

	close(fd_file);

	return 1;
}

int Copy_from(connection * bfd, char * message){

}
