#include "../headers/server.h"

int Disconnected(connection * bfd){

	int file = open(".data.txt", O_CREAT | O_TRUNC | O_RDWR, 0700);		log_perror("open in Disconnected");

	//printf("Write down new data\n");

	for (size_t i = 0; i < bfd->c_users; i++){
		//printf("%s#%s#%s#%s\n", bfd->users[i]->login, bfd->users[i]->password, bfd->users[i]->key, bfd->users[i]->IV);
		dprintf(file, "%s#%s#%s#%s\n", bfd->users[i]->login, bfd->users[i]->password, bfd->users[i]->key, bfd->users[i]->IV);		log_perror("dprintf in Disconnected");
	}
	
	close(file);		log_perror("close in Disconnected");

	return 0;
}

int Login(connection * bfd, char * log_pas){
	// log_pas "__LOGIN__#__PASSWORD__"

	char username[MAX_COMMAND_LENGHT] = {0};
	char password[MAX_COMMAND_LENGHT] = {0};

	char * border = strchr(log_pas, '#');		log_perror("strchr in Login");
	border[0] = '\0';
	strcpy(username, log_pas);		log_perror("strcpy 1 in Login");
	strcpy(password, border + 1);		log_perror("strcpy 2 in Login");

	for (size_t i = 0; i < bfd->c_users; i++){
		if ((strcmp(bfd->users[i]->login, username) == 0) && (strcmp(bfd->users[i]->password, password) == 0)){
			char message[MAX_COMMAND_LENGHT] = "@Success";
			/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));		log_perror("sendto 1 in Login");
			//printf("In bfd %s %s\n", bfd->key, bfd->iv);
			bfd->users[i]->IV = bfd->iv;
			bfd->users[i]->key = bfd->key;
			//printf("In bfd %s %s\n", bfd->users[i]->key, bfd->users[i]->IV);
			return 1;
		}
	}
	printf("Wrong login or password!\n");
	char message[MAX_COMMAND_LENGHT] = "@Wrong username or password!";
	/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));		log_perror("sendto 2 in Login");
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

	strcpy(old_key, border_start+1);		log_perror("strcpy 1 in Check previos session");
	strcpy(old_iv, border_between+1);		log_perror("strcpy 2 in Check previos session");

	for (size_t i = 0; i < bfd->c_users; i++){
		printf("Compare\n%s and %s\n\%s and %s\n", bfd->users[i]->key_old, old_key, bfd->users[i]->IV_old, old_iv);		log_perror("strcpy 1 in Check previos session");
		printf("results %d %d\n",strcmp(bfd->users[i]->key_old, old_key), strcmp(bfd->users[i]->IV_old, old_iv));		log_perror("strcpy 1 in Check previos session");
		if ((strcmp(bfd->users[i]->key_old, old_key) == 0) && (strcmp(bfd->users[i]->IV_old, old_iv) == 0)){
			//printf("Matched login and password!!!\n");
			char message[MAX_COMMAND_LENGHT] = "@Success";
			/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));		log_perror("sendto 1 in Check previos session");
			//printf("In bfd %s %s\n", bfd->key, bfd->iv);
			bfd->users[i]->IV = bfd->iv;
			bfd->users[i]->key = bfd->key;
			//printf("In bfd %s %s\n", bfd->users[i]->key, bfd->users[i]->IV);
			return 1;
		}
	}
	char message[MAX_COMMAND_LENGHT] = "@Wrong";
	/**/sendto(bfd->sock_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));		log_perror("sendto 2 in Check previos session");
	return 1;	
}

int Do_usual(connection * bfd, char * command){

	int stat;
	int file_pipes[2];
	if	(pipe(file_pipes) == -1){
		perror("PIPE");
	}
	
	char ** argv = calloc(MAX_USER_COUNT, sizeof(char *));		log_perror("calloc 1 in Do_usual");
	char * space = command;
	argv[0] = command;
	int count = 1;

	while (1){
		space = strchr(space + 1, ' ');		log_perror("strchr in Do_usual");
		if (space == NULL){
			break;
		}
		space[0] = '\0';
		argv[count] = space + 1;
		count++;
		argv[count] = NULL;
	}

	char * output = calloc(MAX_OUTPUT_LENGHT, sizeof(char));		log_perror("calloc 2 in Do_usual");
	int read_c;
	int code = fork();
	if (!code){
		close(file_pipes[0]);
		dup2(file_pipes[1], STDOUT_FILENO);		log_perror("dup out in Do_usual");
		dup2(file_pipes[1], STDERR_FILENO);		log_perror("dup err in Do_usual");
		execvp(argv[0],  argv);
		return -9;
	}else{
		close(file_pipes[1]);		log_perror("close in Do_usual");
		wait(&stat);		log_perror("wait in Do_usual");
		read_c = read(file_pipes[0], output, MAX_OUTPUT_LENGHT);		log_perror("read in Do_usual");
	}

	/**/sendto(bfd->sock_fd, output, read_c, MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));		log_perror("sendto in Do_usual");
	return 1;
}

int Copy_to(connection * bfd, char * message){
	// __TARGET_PATH__
	// __SIZE__
	// __TEXT__

	int fd_file = open(message, O_CREAT | O_TRUNC | O_RDWR, 0777);		log_perror("open in Copy_to");

	char size_str[MAX_COMMAND_LENGHT] = {0};

	socklen_t len = sizeof(bfd->client);
	recvfrom(bfd->sock_fd, size_str, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&bfd->client, &len);		log_perror("recvfrom 1 in Copy_to");
	int size = atoi(size_str);		log_perror("atoi in Copy_to");

	char * text = calloc(size, sizeof(char));		log_perror("calloc in Copy_to");
	recvfrom(bfd->sock_fd, text, size, MSG_WAITALL, (struct sockaddr *)&bfd->client, &len);		log_perror("recvfrom 2 in Copy_to");

	write(fd_file, text, size);		log_perror("write in Copy_to");

	char answer[MAX_COMMAND_LENGHT] = "@Success";
	/**/sendto(bfd->sock_fd, answer, strlen(answer), MSG_CONFIRM, (const struct sockaddr *)&bfd->client, sizeof(bfd->client));		log_perror("sendto in COpy_to");

	close(fd_file);		log_perror("close in Copy_to");

	return 1;
}

int Copy_from(connection * bfd, char * message){

}
