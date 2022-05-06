#include "../headers/server.h"

void Get_message(connection * bfd, char * message) {

	memset(message, '\0', MAX_COMMAND_LENGHT);		log_perror("memset in Get_message");
	int n;
	socklen_t len = sizeof(bfd->client);

	//printf("Info about connection:\n");
	//printf("Discriptor - %d\n", bfd->sock_fd);
	//printf("Client - %s:%d\n", inet_ntoa(bfd->client.sin_addr), htons(bfd->client.sin_port));
	//printf("Keys - %s %s\n", bfd->key, bfd->iv);
	//printf("Database\n");
	//for (size_t i = 0; i < bfd->c_users; i++){
	//	printf("%s %s %s %s\n", bfd->users[i]->login, bfd->users[i]->password, bfd->users[i]->key_old, bfd->users[i]->IV_old);
	//}printf("\n");

	//printf("Wait for a command...\n");
	n = recvfrom(bfd->sock_fd, (char *)message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&bfd->client, &len);		log_perror("recvfrom in Get_message");
	//printf("Wow, lets go!\n");
	//printf("I recived command\n%s\n", message);
	return;
}

int Parser(char * message, char * content, connection * bfd){
	// 0 - @Disconnected
	// 1 - @Login __LOGIN__#__PASSWORD__
	// 2 - @Have_previos_session :__LOGIN__#__PASSWORD__#__OLD_KEY__#__OLD_IV__
	// 3 - @# {exec}
	// 4 - @Copy_to
	// 5 - @Copy_from

	//printf("Allright, lets check this stuff...\n%s\n", message);

	char * space = strchr(message, ' ');		log_perror("strchr");
	if (space == NULL){
		if (strcmp(message, "@Disconnected") == 0){
			return Disconnected(bfd);
		}
	}
	
	space[0] = '\0';
	char cmd[MAX_COMMAND_LENGHT] = {0};
	char args[MAX_COMMAND_LENGHT] = {0};
	strcpy(cmd, message);		log_perror("strcpy 1 in Parser");
	strcpy(args, space + 1);		log_perror("strcpy 2 in Parser");

	if (strcmp(cmd, "@Login") == 0){
		log_info("Get command: @Login");
		//return Login(bfd, args);
	}else if (strcmp(cmd, "@Have_previos_session") == 0){
		log_info("Get command: @Have_previos_session\n");
		//return Check_previos_session(bfd, args);
	}else if (strcmp(cmd, "@#") == 0){
		log_info("Get command: @#");
		return Do_usual(bfd, args);
	}else if (strcmp(cmd, "@Copy_to") == 0){
		log_info("Get command: @Copy_to");
		return Copy_to(bfd, args);
	}else if (strcmp(cmd, "@Copy_from") == 0){
		log_info("Get command: @Copy_from");
		return Copy_from(bfd, args);
	}else{
		log_error("Get unknown command");
	}
	log_perror("strcmp in Parser");
	return -1; // ERROR
}