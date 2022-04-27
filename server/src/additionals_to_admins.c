#include "../headers/server.h"

connection Get_database (){

	user ** database = calloc(MAX_USER_COUNT, sizeof(user *));

	int data = open(".data.txt", O_CREAT, 0700);
	// Many string like "__LOGIN__#__PASSWORD__#__OLD_KEY__#__OLD_IV__"

	char temp[MAX_COMMAND_LENGHT * MAX_USER_COUNT] = {0};
	read(data, temp, MAX_COMMAND_LENGHT * MAX_USER_COUNT);

	char * check = (char *)&temp; // Just noNULL pointer
	int count = 0;

	while (check != NULL){
		database[count] = Get_user(check);
		count++;
		check = strchr(check + 1, '\n');
	}

	connection ret = {};

	ret.c_users = count;
	ret.users = database;	

	close(data);
	return ret;
}

user * Get_user(char * data){
	// String like "__LOGIN__#__PASSWORD__#__OLD_KEY__#__OLD_IV__"
	user pers = {};

	char * temp;
	pers.login = data;

	temp = strchr(data, '#');	
	temp[0] = '\0';
	pers.password = temp + 1;
	temp = strchr(pers.password, '#');
	temp[0] = '\0';
	pers.key_old = temp + 1;
	temp = strchr(pers.key_old, '#');
	temp[0] = '\0';
	pers.IV_old = temp + 1;
	user * ptr = &pers;
	return ptr;
}

void Get_message(connection * bfd, char * message) {
	char buf[MAX_COMMAND_LENGHT];
	int n;
	socklen_t len = sizeof(bfd->client);
	memset(buf, 0, sizeof(buf));
	n = recvfrom(bfd->sock_fd, (char *)buf, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&bfd->client, &len);
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

	if (strcmp(cmd, "@Log_in") == 0){
		return Login(bfd, args);
	}else if (strcmp(cmd, "@Have_previos_session") == 0){
		return Check_previos_session(bfd, args);
	}else if (strcmp(cmd, "@#") == 0){
		return Do_usual(bfd, args);
	}else if (strcmp(cmd, "@Copy_to") == 0){
		return Copy_to();
	}else if (strcmp(cmd, "@Copy_from") == 0){
		return Copy_from();
	}else{
		// ERROR
	}
	return -1; // ERROR
}