#include "../headers/server.h"

void Preparing_keys(int sock_fd, struct sockaddr_in client){
	int a = rand() / 0b1111111111111111;
	int A = Speed_degree_with_mod(PUBLIC_KEY_G, a, PUBLIC_KEY_P);

	char client_message[MAX_COMMAND_LENGHT] = {0};
	char greetings[MAX_COMMAND_LENGHT] = {0};

	sprintf(greetings, "@You are not alone!\n%d\n%d\n%d\n", PUBLIC_KEY_P, PUBLIC_KEY_G, A);

	sendto(sock_fd, (const char *)greetings, strlen(greetings), MSG_CONFIRM, (const struct sockaddr *)&client, sizeof(client));

	int n,len;
	n = recvfrom(sock_fd, (char *)client_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&client, &len);
	client_message[n] = '\0';

	// Message form: "@secret info:__KEY__"
	char * place_B = strchr(client_message, ':');

	int B = atoi(place_B + 1);
	int K = Speed_degree_with_mod(PUBLIC_KEY_G, B, PUBLIC_KEY_P);

	printf("KEY %d\n", K);
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