#include "../headers/server.h"

connection * Get_database (){

	user ** database = calloc(MAX_USER_COUNT, sizeof(user *));		log_perror("calloc in Get_database");

	int data = open(".data.txt", O_CREAT | O_RDWR, 0700);		log_perror("open .data.txt in Get_database");
	// Many string like "__LOGIN__#__PASSWORD__#__OLD_KEY__#__OLD_IV__"

	char temp[MAX_COMMAND_LENGHT * MAX_USER_COUNT] = {0};
	read(data, temp, MAX_COMMAND_LENGHT * MAX_USER_COUNT);		log_perror("read .data.txt in Get_database");

	char * check = (char *)&temp; // Just noNULL pointer
	char * end = strchr(check + 1, '\n');		log_perror("strchr in Get_database");
	end[0] = '\0';
	int count = 0;


	while (1){
		database[count] = Get_user(check);
		count++;
		check = end + 1;
		end = strchr(check + 1, '\n');		log_perror("strchr in Ger_database");
		if (end == NULL){
			break;			
		}
		end[0] = '\0';	
	}

	connection * ret = calloc(1, sizeof(connection));

	ret->c_users = count;
	ret->users = database;	

	close(data);		log_perror("close in Get_database");
	return ret;
}

user * Get_user(char * data){
	//printf("Get string\n%s\n", data);
	// String like "__LOGIN__#__PASSWORD__#__OLD_KEY__#__OLD_IV__"
	user * pers = calloc(1, sizeof(user));		log_perror("calloc in Get_user");

	char * temp;
	pers->login = data;

	temp = strchr(data, '#');		log_perror("strchr 1 in Get_user");
	temp[0] = '\0';
	pers->password = temp + 1;
	temp = strchr(pers->password, '#');		log_perror("strchr 2 in Get_user");
	temp[0] = '\0';
	pers->key_old = temp + 1;
	temp = strchr(pers->key_old, '#');		log_perror("strchr 2 in Get_user");
	temp[0] = '\0';
	pers->IV_old = temp + 1;
	user * ptr = pers;
	//printf("%s>%s>%s>%s\n", pers->login, pers->password, pers->key_old, pers->IV_old);
	return ptr;
}

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
		return Login(bfd, args);
	}else if (strcmp(cmd, "@Have_previos_session") == 0){
		log_info("Get command: @Have_previos_session\n");
		return Check_previos_session(bfd, args);
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