#include "../headers/server.h"

connection * Get_database (){

	user ** database = calloc(MAX_USER_COUNT, sizeof(user *));

	int data = open(".data.txt", O_CREAT, 0700);
	// Many string like "__LOGIN__#__PASSWORD__#__OLD_KEY__#__OLD_IV__"

	char temp[MAX_COMMAND_LENGHT * MAX_USER_COUNT] = {0};
	read(data, temp, MAX_COMMAND_LENGHT * MAX_USER_COUNT);

	char * check = (char *)&temp; // Just noNULL pointer
	char * end = strchr(check + 1, '\n');
	end[0] = '\0';
	int count = 0;

	while (1){
		database[count] = Get_user(check);
		count++;
		check = end + 1;
		end = strchr(check + 1, '\n');
		if (end == NULL){
			break;			
		}
		end[0] = '\0';	
	}
	printf("Done make database!\n");

	connection * ret = calloc(1, sizeof(connection));

	ret->c_users = count;
	ret->users = database;	

	
	printf("Info about connection:\n");
	printf("Client - %s:%d\n", inet_ntoa(ret->client.sin_addr), htons(ret->client.sin_port));
	printf("Keys - %s %s\n", ret->key, ret->iv);
	printf("Database\n");
	for (size_t i = 0; i < ret->c_users; i++){
		printf("%s %s %s %s\n", ret->users[i]->login, ret->users[i]->password, ret->users[i]->key_old, ret->users[i]->IV_old);
	}printf("\n");

	close(data);
	return ret;
}

user * Get_user(char * data){
	printf("Get string\n%s\n", data);
	// String like "__LOGIN__#__PASSWORD__#__OLD_KEY__#__OLD_IV__"
	user * pers = calloc(1, sizeof(user));

	char * temp;
	pers->login = data;

	temp = strchr(data, '#');	
	temp[0] = '\0';
	pers->password = temp + 1;
	temp = strchr(pers->password, '#');
	temp[0] = '\0';
	pers->key_old = temp + 1;
	temp = strchr(pers->key_old, '#');
	temp[0] = '\0';
	pers->IV_old = temp + 1;
	user * ptr = pers;
	printf("%s>%s>%s>%s\n", pers->login, pers->password, pers->key_old, pers->IV_old);
	return ptr;
}

void Get_message(connection * bfd, char * message) {

	char buf[MAX_COMMAND_LENGHT];
	int n;
	socklen_t len = sizeof(bfd->client);
	memset(buf, 0, sizeof(buf));
	
	printf("Info about connection:\n");
	printf("Client - %s:%d\n", inet_ntoa(bfd->client.sin_addr), htons(bfd->client.sin_port));
	printf("Keys - %s %s\n", bfd->key, bfd->iv);
	printf("Database\n");
	for (size_t i = 0; i < bfd->c_users; i++){
		printf("%s %s %s %s\n", bfd->users[i]->login, bfd->users[i]->password, bfd->users[i]->key_old, bfd->users[i]->IV_old);
	}printf("\n");

	printf("I AM READY!!!\n");
	n = recvfrom(bfd->sock_fd, (char *)buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *)&bfd->client, &len);
	printf("Recieve message:\n%s\n", message);
	return;
}

int Parser(char * message, char * content, connection * bfd){
	// 0 - @Disconnected
	// 1 - @Log_in __LOGIN__#__PASSWORD__
	// 2 - @Have_previos_session :__LOGIN__#__PASSWORD__#__OLD_KEY__#__OLD_IV__
	// 3 - @# {exec}
	// 4 - @Copy_to
	// 5 - @Copy_from

	char * space = strchr(message, ' ');
	if (space == NULL){
		if (strcmp(message, "@Disconnected") == 0){
			return Disconnected();
		}
	}	
	
	space[0] = '\0';
	char cmd[MAX_COMMAND_LENGHT] = {0};
	char args[MAX_COMMAND_LENGHT] = {0};
	strcpy(cmd, message);
	strcpy(args, space + 1);

	if (strcmp(cmd, "@Login") == 0){
		printf("Get command: @Login\n");
		return Login(bfd, args);
	}else if (strcmp(cmd, "@Have_previos_session") == 0){
		printf("Get command: @Have_previos_session\n");
		return Check_previos_session(bfd, args);
	}else if (strcmp(cmd, "@#") == 0){
		printf("Get command: @#\n");
		return Do_usual(bfd, args);
	}else if (strcmp(cmd, "@Copy_to") == 0){
		printf("Get command: @Copy_to\n");
		return Copy_to();
	}else if (strcmp(cmd, "@Copy_from") == 0){
		printf("Get command: @Copy_from\n");
		return Copy_from();
	}else{
		printf("Unknown_command");
	}
	return -1; // ERROR
}