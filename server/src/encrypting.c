#include "../headers/server.h"

void Preparing_numeral_keys(int mode, connection * bfd, size_t * K1, size_t * K2){

	srand(time(0));		log_perror("srand in Prepare keys");

	printf("GO SECURE\n");

	size_t a1 = rand() % 0b11111111111111111111111111111111;		log_perror("rand 1 in Prepare keys");
	size_t a2 = rand() % 0b11111111111111111111111111111111;		log_perror("rand 2 in Prepare keys");

	//printf("Gen:\n%zu\n%zu\n", a1,a2);	

	size_t A1 = Speed_degree_with_mod(PUBLIC_KEY_G, a1, PUBLIC_KEY_P);
	size_t A2 = Speed_degree_with_mod(PUBLIC_KEY_G, a2, PUBLIC_KEY_P);

	//printf("Get:\nA1 = %zu\nA2 = %zu\n", A1, A2);

	char client_message[MAX_COMMAND_LENGHT] = {0};
	char greetings[MAX_COMMAND_LENGHT] = {0};

	sprintf(greetings, "@You are not alone!:%d#%d#%zu#%zu\n", PUBLIC_KEY_P, PUBLIC_KEY_G, A1, A2);		log_perror("sprintf in Prepare keys");
	//usleep(TIMEOUT_BEFORE_SEND_KEYS);

	if (mode){
		write(bfd->conn_fd, greetings, strlen(greetings));	
	}else{
		sendto(bfd->sock_fd, (const char *)greetings, strlen(greetings), MSG_CONFIRM, (const struct sockaddr *)&(bfd->client), sizeof(bfd->client));		log_perror("sendto in Prepare keys");
	}


	int n;
	unsigned int len;
	if (mode){
		n = read(bfd->conn_fd, client_message, MAX_COMMAND_LENGHT);
	}else{
		n = recvfrom(bfd->sock_fd, (char *)client_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&(bfd->client), &len);		log_perror("recvfrom in Prepare keys");
	}
	client_message[n] = '\0';
	
	//printf("GET:\n%s\n", client_message);

	// Message form: "@Secret info:__KEY_1__:__KEY_2__"
	char * place_B1 = strchr(client_message + 1, ':');		log_perror("strchr 1 in Prepare keys");
	char * place_B2 = strchr(place_B1 + 1,':');		log_perror("strchr 2 in Prepare keys");

	//printf("CHECK\n");
	size_t B1 = atoi(place_B1 + 1);		log_perror("atoi 1 in Prepare keys");
	size_t B2 = atoi(place_B2 + 1);		log_perror("atoi 2 in Prepare keys");

	//printf("Get:\nB1 = %zu\nB2 = %zu\n", B1, B2);

	*K1 = Speed_degree_with_mod(B1, a1, PUBLIC_KEY_P);
	*K2 = Speed_degree_with_mod(B2, a2, PUBLIC_KEY_P);

	printf("KEYS:\n%zu\n%zu\n", *K1, *K2);
}

size_t Speed_degree_with_mod(size_t g, size_t x, size_t p){ // return = g^x % p
	if (x == 0)
		return 1;

	if (x % 2 != 0)
		return (Speed_degree_with_mod(g, x - 1, p) * g) % p;

	size_t y = Speed_degree_with_mod(g, x / 2, p);
	return (y * y) % p;
}

void Make_keys(size_t K1, size_t K2, char * key, char * IV){
	char A[MAX_COMMAND_LENGHT] = {0};
	char B[MAX_COMMAND_LENGHT] = {0};

	sprintf(A, "%zu%zu%zu%zu%zu%zu%zu%zu", K1, K2, K1, K2, K1, K2, K1, K2);		log_perror("sprintf 1 in Make keys");
	sprintf(B, "%zu%zu%zu%zu%zu%zu%zu%zu", K2, K1, K2, K1, K2, K1, K2, K1);		log_perror("sprintf 2 in Make keys");

	//printf("A: %s\n", A);
	//printf("B: %s\n", B);
	
	strcat(key, A);		log_perror("strcat 1 in Make keys");
	strcat(IV, B);		log_perror("strcat 2 in Make keys");

	for (size_t i = 32; i < MAX_COMMAND_LENGHT; i++){
		key[i] = '\0';
	}
	
	for (size_t i = 16; i < MAX_COMMAND_LENGHT; i++){
		IV[i] = '\0';
	}

	printf("key: %s\n", key);
	printf("IV: %s\n", IV);
}

int Encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext){
	EVP_CIPHER_CTX *ctx;

	int len;

	int ciphertext_len;

	/* Create and initialise the context */
	ctx = EVP_CIPHER_CTX_new();

	/*
	 * Initialise the encryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

	/*
	 * Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_EncryptUpdate can be called multiple times if necessary
	 */
	EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
	ciphertext_len = len;

	/*
	 * Finalise the encryption. Further ciphertext bytes may be written at
	 * this stage.
	 */
	EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
	ciphertext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}


int Decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext){
	EVP_CIPHER_CTX *ctx;

	int len;

	int plaintext_len;

	/* Create and initialise the context */
	ctx = EVP_CIPHER_CTX_new();

	/*
	 * Initialise the decryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);


	/*
	 * Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary.
	 */
	EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
	plaintext_len = len;

	/*
	 * Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
	plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}
