#include "../headers/server.h"

void Preparing_numeral_keys(int sock_fd, SSI client){

	int a1 = rand() / 0b11111111111111111111111111111111;
	int a2 = rand() / 0b11111111111111111111111111111111;
	int A1 = Speed_degree_with_mod(PUBLIC_KEY_G, a1, PUBLIC_KEY_P);
	int A2 = Speed_degree_with_mod(PUBLIC_KEY_G, a2, PUBLIC_KEY_P);

	char client_message[MAX_COMMAND_LENGHT] = {0};
	char greetings[MAX_COMMAND_LENGHT] = {0};

	sprintf(greetings, "@You are not alone!:%d:%d:%d:%d\n", PUBLIC_KEY_P, PUBLIC_KEY_G, A1, A2);


	usleep(TIMEOUT_BEFORE_SEND_KEYS);
	sendto(sock_fd, (const char *)greetings, strlen(greetings), MSG_CONFIRM, (const struct sockaddr *)&client, sizeof(client));

	int n;
	unsigned int len;
	n = recvfrom(sock_fd, (char *)client_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&client, &len);
	client_message[n] = '\0';

	// Message form: "@Secret info:__KEY_1__:__KEY_2__"
	char * place_B1 = strchr(client_message + 1, ':');
	char * place_B2 = strchr(place_B1 + 1,':');

	int B1 = atoi(place_B1 + 1);
	int B2 = atoi(place_B2 + 1);
	int K1 = Speed_degree_with_mod(PUBLIC_KEY_G, B1, PUBLIC_KEY_P);
	int K2 = Speed_degree_with_mod(PUBLIC_KEY_G, B2, PUBLIC_KEY_P);

	printf("KEYS:\n%d\n%d\n", K1, K2);
}

void Encryption(){

}

void Decoding(){

}

int Speed_degree_with_mod(int g, int x, int p){ // return = g^x % p
	if (x == 0)
		return 1;

	if (x % 2 != 0)
		return (Speed_degree_with_mod(g, x - 1, p) * g) % p;

	int y = Speed_degree_with_mod(g, x / 2, p);
	return (y * y) % p;
}