#include "../headers/server.h"

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