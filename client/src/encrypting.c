#include "../headers/client.h"

void Preparing_numeral_keys(int sock_fd, SSI admin){

	printf("GO SECURE!!!\n");

	int n;
	unsigned int len;
	char admin_message[MAX_COMMAND_LENGHT] = {0};
	n = recvfrom(sock_fd, (char *)admin_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&admin, &len);
	admin_message[n] = '\0';

	int b1 = rand() / 0b11111111111111111111111111111111;
	int b2 = rand() / 0b11111111111111111111111111111111;

	char answer[MAX_COMMAND_LENGHT] = {0};

	// Message form: @You are not alone!:__P_KEY_G__:__P_KEY_P__:__A1__:__A2__
	
	//printf("Get message:\n%s\n", admin_message);

	char * place_P = strchr(admin_message + 1, ':');
	char * place_G = strchr(place_P + 1, ':');
	char * place_A1 = strchr(place_G + 1, ':');
	char * place_A2 = strchr(place_A1 + 1, ':');
	
	//printf("\n\n%s\n%s\n%s\n%s\n",place_G+1, place_P+1, place_A1+1, place_A2+1);

	int KEY_G = atoi(place_G + 1);
	int KEY_P = atoi(place_P + 1);
	int A1 = atoi(place_A1 + 1);
	int A2 = atoi(place_A2 + 1);

	printf("Get:\nG = %d\nP = %d\n", KEY_G, KEY_P);
	printf("Get:\nA1 = %d\nA2 = %d\n", A1, A2);

	int B1 = Speed_degree_with_mod(KEY_G, b1, KEY_P);
	int B2 = Speed_degree_with_mod(KEY_G, b2, KEY_P);

	printf("Get:\nB1 = %d\nB2 = %d\n", B1, B2);

	int K1 = Speed_degree_with_mod(KEY_G, A1, KEY_P);
	int K2 = Speed_degree_with_mod(KEY_G, A2, KEY_P);

	sprintf(answer, "@Secret info:%d:%d", B1, B2);

	sendto(sock_fd, (const char *)answer, strlen(answer), MSG_CONFIRM, (const struct sockaddr *)&admin, sizeof(admin));

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