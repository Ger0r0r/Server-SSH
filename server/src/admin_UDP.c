#include "../headers/server.h"

user ** Get_database (){

	user ** database = calloc(MAX_USER_COUNT, sizeof(*user));

	int data = open(".data.txt", O_CREAT, 0700);
	// Many string like "__LOGIN__#__PASSWORD__#__OLD_KEY__#__OLD_IV__"

	char temp[MAX_COMMAND_LENGHT * MAX_USER_COUNT] = {0};
	read(data, temp, MAX_COMMAND_LENGHT * MAX_USER_COUNT);

	char * check = &temp; // Just noNULL pointer
	int count = 0;

	while (check != NULL){
		database[count] = Get_user(check);
		count++;
		check = strchr(check, '\n');
	}

	close(data);
	return database;
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
	return &pers;
}

char * Get_message() {

}