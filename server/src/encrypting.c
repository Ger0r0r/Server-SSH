#include "../headers/server.h"

void Preparing_numeral_keys(int sock_fd, SSI client){

	srand(time(0));

	size_t a1 = rand() % 0b11111111111111111111111111111111;
	size_t a2 = rand() % 0b11111111111111111111111111111111;

	

	size_t A1 = Speed_degree_with_mod(PUBLIC_KEY_G, a1, PUBLIC_KEY_P);
	size_t A2 = Speed_degree_with_mod(PUBLIC_KEY_G, a2, PUBLIC_KEY_P);

	printf("Get:\nA1 = %zu\nA2 = %zu\n", A1, A2);

	char client_message[MAX_COMMAND_LENGHT] = {0};
	char greetings[MAX_COMMAND_LENGHT] = {0};

	sprintf(greetings, "@You are not alone!:%d:%d:%zu:%zu\n", PUBLIC_KEY_P, PUBLIC_KEY_G, A1, A2);


	usleep(TIMEOUT_BEFORE_SEND_KEYS);
	sendto(sock_fd, (const char *)greetings, strlen(greetings), MSG_CONFIRM, (const struct sockaddr *)&client, sizeof(client));

	int n;
	unsigned int len;
	n = recvfrom(sock_fd, (char *)client_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&client, &len);
	client_message[n] = '\0';

	// Message form: "@Secret info:__KEY_1__:__KEY_2__"
	char * place_B1 = strchr(client_message + 1, ':');
	char * place_B2 = strchr(place_B1 + 1,':');

	size_t B1 = atoi(place_B1 + 1);
	size_t B2 = atoi(place_B2 + 1);

	printf("Get:\nB1 = %zu\nB2 = %zu\n", B1, B2);

	size_t K1 = Speed_degree_with_mod(B1, a1, PUBLIC_KEY_P);
	size_t K2 = Speed_degree_with_mod(B2, a2, PUBLIC_KEY_P);

	printf("KEYS:\n%zu\n%zu\n", K1, K2);
}

void Encryption(){

}

void Decoding(){

}

size_t Speed_degree_with_mod(size_t g, size_t x, size_t p){ // return = g^x % p
	if (x == 0)
		return 1;

	if (x % 2 != 0)
		return (Speed_degree_with_mod(g, x - 1, p) * g) % p;

	size_t y = Speed_degree_with_mod(g, x / 2, p);
	return (y * y) % p;
}