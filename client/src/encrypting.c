#include "../headers/client.h"

void Preparing_numeral_keys(int sock_fd, SSI admin, size_t * K1, size_t * K2){

	//printf("GO SECURE!!!\n");

	int n;
	unsigned int len;
	char admin_message[MAX_COMMAND_LENGHT] = {0};
	n = recvfrom(sock_fd, (char *)admin_message, MAX_COMMAND_LENGHT, MSG_WAITALL, (struct sockaddr *)&admin, &len);
	admin_message[n] = '\0';

	srand(time(0));

	size_t b1 = rand() % 0b11111111111111111111111111111111;
	size_t b2 = rand() % 0b11111111111111111111111111111111;

	//printf("Gen:\n%zu\n%zu\n", b1,b2);

	char answer[MAX_COMMAND_LENGHT] = {0};

	// Message form: @You are not alone!:__P_KEY_G__#__P_KEY_P__#__A1__#__A2__
	
	//printf("Get message:\n%s\n", admin_message);

	char * place_P = strchr(admin_message + 1, ':');
	char * place_G = strchr(place_P + 1, '#');
	char * place_A1 = strchr(place_G + 1, '#');
	char * place_A2 = strchr(place_A1 + 1, '#');
	
	//printf("\n\n%s\n%s\n%s\n%s\n",place_G+1, place_P+1, place_A1+1, place_A2+1);

	//printf("Wow,.. timeout...\n");
	size_t KEY_G = atoi(place_G + 1);
	size_t KEY_P = atoi(place_P + 1);
	size_t A1 = atoi(place_A1 + 1);
	size_t A2 = atoi(place_A2 + 1);

	//printf("Get:\nG = %zu\nP = %zu\n", KEY_G, KEY_P);
	//printf("Get:\nA1 = %zu\nA2 = %zu\n", A1, A2);

	size_t B1 = Speed_degree_with_mod(KEY_G, b1, KEY_P);
	size_t B2 = Speed_degree_with_mod(KEY_G, b2, KEY_P);

	//printf("Get:\nB1 = %zu\nB2 = %zu\n", B1, B2);

	//printf("\n\n\n\nSUKAA\nA1 = %zu\nb1 = %zu\nP = %zu\n", A1, b1, KEY_P);
	*K1 = Speed_degree_with_mod(A1, b1, KEY_P);
	*K2 = Speed_degree_with_mod(A2, b2, KEY_P);

	sprintf(answer, "@Secret info:%zu:%zu", B1, B2);

	sendto(sock_fd, (const char *)answer, strlen(answer), MSG_CONFIRM, (const struct sockaddr *)&admin, sizeof(admin));

	//printf("CHECK FUUCK\n");
	//printf("Descriptor - %d\n", sock_fd);
	//printf("Admin - %s:%d\n", inet_ntoa(admin.sin_addr), htons(admin.sin_port));

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

	sprintf(A, "%zu%zu%zu%zu%zu%zu%zu%zu", K1, K2, K1, K2, K1, K2, K1, K2);
	sprintf(B, "%zu%zu%zu%zu%zu%zu%zu%zu", K2, K1, K2, K1, K2, K1, K2, K1);

	//printf("A: %s\n", A);
	//printf("B: %s\n", B);
	
	strcat(key, A);
	strcat(IV, B);

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
	CHECK(!(ctx = EVP_CIPHER_CTX_new()))

	/*
	 * Initialise the encryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	CHECK(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))

	/*
	 * Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_EncryptUpdate can be called multiple times if necessary
	 */
	CHECK(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
	ciphertext_len = len;

	/*
	 * Finalise the encryption. Further ciphertext bytes may be written at
	 * this stage.
	 */
	CHECK(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
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
	CHECK(!(ctx = EVP_CIPHER_CTX_new()))

	/*
	 * Initialise the decryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	CHECK(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))

	/*
	 * Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary.
	 */
	CHECK(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
	plaintext_len = len;

	/*
	 * Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	CHECK(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
	plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}
