#include "../headers/server.h"

int signal_input;
int current_client = 0;
int mode;

void Wait_connection(int mmm){

	mode = mmm;

	struct sigaction act;
	memset(&act, 0 , sizeof(act));
	act.sa_sigaction = Handler_connection;
	act.sa_flags = SA_SIGINFO;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);	
	act.sa_mask = set;

	sigaction(SIGUSR1, &act, 0);

	while (1){}
	return;
}

void Handler_connection(int sigN, siginfo_t* sigInfo, void* context){
	if (sigN != SIGUSR1)
		return;
	
	if (context == NULL)
		return;

	size_t data = (size_t)sigInfo->si_value.sival_ptr;

	connection new_client = Translate_signal(data);
	
	int pid = fork();

	if (pid == 0){
		printf("%s:%d\n", inet_ntoa(new_client.addr), ntohs(new_client.port));
		current_client++;
		if (mode == 1){ // TCP
			Administraitor_TCP(new_client, current_client);
		}else if (mode == 0){ // UDP
			Administraitor_UDP(new_client, current_client);
		}else{
			// ERROR
		}
		
		return;
	}
	


	return;
}

connection Translate_signal(size_t data){
	connection temp;

	temp.addr.s_addr = htonl(data & 0b11111111111111111111111111111111);
	temp.port = htons((data >> 32) & 0b1111111111111111);

	return temp;
}