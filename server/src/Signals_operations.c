#include "../headers/server.h"

SSI Translate_signal(size_t data){
	SSI temp;

	temp.sin_addr.s_addr = htonl(data & 0b11111111111111111111111111111111);
	temp.sin_port = htons((data >> 32) & 0b1111111111111111);

	return temp;
}

size_t Encrypt_signal(SSI data){
	size_t temp = 0;

	temp |= ntohl(data.sin_addr.s_addr);
	temp |= ((size_t)(ntohs(data.sin_port))<<32);

	return temp;
}