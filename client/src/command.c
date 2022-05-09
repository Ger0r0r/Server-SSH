#include "../headers/client.h"

/*
	COMMANDS
	-3	- connection failed
	-2	- log in failed
	-1	- error
	0	- exit
	1	- log in success
	2	- connection success
*/

int Command_exit(connection * data){
	char * message = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	sprintf(message, "@Disconnected");
	sendto(data->sock_fd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));
	free(message);
	return 0;
}

int Command_exec(connection * data, input enter){
	char message[3 * MAX_COMMAND_LENGHT] = {0};
	sprintf(message, "@# %s %s", enter.cmd, enter.arg);
	/**/sendto(data->sock_fd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));

	int n;
	unsigned int len;
	char admin_message[MAX_OUTPUT_LENGHT] = {0};
	/**/n = recvfrom(data->sock_fd, (char *)admin_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&data->admin, &len);
	admin_message[n] = '\0';

	printf("\n%s\n", admin_message);
	return 1;
}

int Command_copy_to(connection * data, char * message){
	// message "__PATH_FROM_CLIENT__ __PATH_TO_SERVER__"

	char * space = strchr(message, ' ');
	space[0] = '\0';
	char * path_from = message;
	char * path_to = space + 1;

	int fd_from = open(path_from, O_RDONLY);

	struct stat * buf = calloc(1, sizeof(struct stat));

	fstat(fd_from, buf);

	char first_send[MAX_COMMAND_LENGHT] = {0};
	char size[MAX_COMMAND_LENGHT] = {0};
	sprintf(first_send, "@Copy_to %s", path_to);
	sprintf(size, "%ld", buf->st_size);

	printf("\nSIZE %ld\n", buf->st_size);

	char * text = calloc(buf->st_size, sizeof(char));

	read(fd_from, text, buf->st_size);

	/**/sendto(data->sock_fd, first_send, strlen(first_send), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));
	usleep(100);
	/**/sendto(data->sock_fd, size, strlen(size), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));
	usleep(100);
	/**/sendto(data->sock_fd, text, buf->st_size, MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));
	usleep(100);

	int n;
	unsigned int len;
	char admin_message[MAX_OUTPUT_LENGHT] = {0};
	/**/n = recvfrom(data->sock_fd, (char *)admin_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&data->client, &len);
	admin_message[n] = '\0';

	printf("%s\n", admin_message);

	close(fd_from);

	return 1;
}

int Command_copy_from(connection * data, char * message){

}
