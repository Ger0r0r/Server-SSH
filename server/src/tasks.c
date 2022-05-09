#include "../headers/server.h"

int Disconnected(connection * bfd){

	return 0;
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
