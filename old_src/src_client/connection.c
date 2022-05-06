#include "../headers/client.h"

int Connection_attempt(SSI own_addr, SSI * adm_addr){

	memset(adm_addr, 0, sizeof(*adm_addr));

	int ret = Broadcast_find(own_addr, adm_addr);

	if (ret){
		printf("Get admin %s:%d\n", inet_ntoa(adm_addr->sin_addr), ntohs(adm_addr->sin_port));
		return 1;
	}else{
		printf("\n\tConnection failed\n\n");
	}
	return 0;
}

void Generetion_keys(connection * data){
	char * hey = "@Hey admin!\n";
	sendto(data->sock_fd, (const char *)hey, strlen(hey), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));

	size_t K1, K2;
	Preparing_numeral_keys(data->sock_fd, data->admin, &K1, &K2);
	char * Key_new = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	char * IV_new = calloc(MAX_COMMAND_LENGHT, sizeof(char));
	Make_keys(K1, K2, Key_new, IV_new);
	data->info_user.key = Key_new;
	data->info_user.IV = IV_new;
	//printf("In data\n%s %s\n", data->info_user.key, data->info_user.IV);
}

int Check_for_old_keys(connection * data){
	int secret_txt = open(".login.txt", O_CREAT | O_RDWR, 0700);

	char buf[MAX_COMMAND_LENGHT] = {0};

	// in .login.txt "__LOGIN__#__PASSWORD__#__KEY__#__IV__"

	int count = read(secret_txt, buf, MAX_COMMAND_LENGHT);
	if (count < 0){
		perror("Read secret.txt");
	}else if (count > 32){
		char * get_password = strchr(buf, '#');
		char * get_key_old = strchr(get_password + 1, '#');
		char * get_IV_old = strchr(get_key_old + 1, '#');

		buf[get_password - buf] = '\0';
		buf[get_key_old - buf] = '\0';
		buf[get_IV_old - buf] = '\0';

		char * login = calloc(MAX_COMMAND_LENGHT, sizeof(char));
		char * password = calloc(MAX_COMMAND_LENGHT, sizeof(char));
		char * key_old = calloc(MAX_COMMAND_LENGHT, sizeof(char));
		char * iv_old = calloc(MAX_COMMAND_LENGHT, sizeof(char));

		strcpy(login, buf);
		strcpy(password, get_password + 1);
		strcpy(key_old, get_key_old + 1);
		strcpy(iv_old, get_IV_old + 1);

		printf("HOLY SHIT!!!\n");

		data->info_user.login = login;
		data->info_user.password = password;
		data->info_user.key_old = key_old;
		data->info_user.IV_old = iv_old;

		printf("Read:\nlogin %s\npassword %s\nold key %s\nold IV %s\n", data->info_user.login, data->info_user.password, data->info_user.key_old, data->info_user.IV_old);
		close(secret_txt);
		return 1;		

	}else{
		printf("Previos session keys does'n exists\n");
		close(secret_txt);
		return -1;		
	}
	return -1;
}

int Auto_login(connection * data){
	char another_ses[MAX_COMMAND_LENGHT] = {0};
	sprintf(another_ses, "@Have_previos_session %s#%s#%s#%s\n", data->info_user.login, data->info_user.password, data->info_user.key_old, data->info_user.IV_old);
	/**/sendto(data->sock_fd, (const char *)another_ses, strlen(another_ses), MSG_CONFIRM, (const struct sockaddr *)&data->admin, sizeof(data->admin));

	int n;
	unsigned int len;
	char admin_message[MAX_COMMAND_LENGHT] = {0};
	/**/n = recvfrom(data->sock_fd, (char *)admin_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&data->admin, &len);
	admin_message[n] = '\0';

	if (strcmp(admin_message, "@Success") == 0){
		data->status = 2;
		int secret_info = open(".login.txt", O_CREAT, O_TRUNC, 0700);

		dprintf(secret_info, "%s#%s#%s#%s\n", data->info_user.login, data->info_user.password, data->info_user.key, data->info_user.IV);

		close(secret_info);
		return 1;
	}else{
		return 2;
	}
	return -1;
}