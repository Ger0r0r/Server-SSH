#include "../headers/server.h"

int signal_input;
int current_admins = 0;
int mode;
connection * bfd; // Big Faking Database

void Wait_connection(int mmm){

	mode = mmm;

	struct sigaction act_usr1;
	memset(&act_usr1, 0 , sizeof(act_usr1));
	act_usr1.sa_sigaction = Send_admin_info;
	act_usr1.sa_flags = SA_SIGINFO;
	sigset_t set1;
	sigemptyset(&set1);
	sigaddset(&set1, SIGUSR1);	
	act_usr1.sa_mask = set1;

	struct sigaction act_usr2;
	memset(&act_usr2, 0 , sizeof(act_usr2));
	act_usr2.sa_sigaction = New_admin_request;
	act_usr2.sa_flags = SA_SIGINFO;
	sigset_t set2;
	sigemptyset(&set2);
	sigaddset(&set2, SIGUSR1);	
	act_usr2.sa_mask = set2;

	sigaction(SIGUSR1, &act_usr1, 0); // update admin addr
	sigaction(SIGUSR2, &act_usr2, 0); // fork for new admin

	Get_database(bfd);

	kill(getpid(), SIGUSR2);

	while (1){}
	return;
}

void Send_admin_info(int sigN, siginfo_t* sigInfo, void* context){
	if (sigN != SIGUSR1)
		return;
	
	union sigval cn_info;
	memset(&cn_info, 0, sizeof(cn_info));
	cn_info.sival_ptr = sigInfo->si_value.sival_ptr;
	sigqueue(getppid(), SIGUSR1, cn_info);
	
	return;
}

void New_admin_request(int sigN, siginfo_t* sigInfo, void* context){
	if (sigN != SIGUSR2)
		return;

	printf("Info about connection:\n");
	printf("Client - %s:%d\n", inet_ntoa(bfd->client.sin_addr), htons(bfd->client.sin_port));
	printf("Keys - %s %s\n", bfd->key, bfd->iv);
	printf("Database\n");
	for (size_t i = 0; i < bfd->c_users; i++){
		printf("%s %s %s %s\n", bfd->users[i]->login, bfd->users[i]->password, bfd->users[i]->key_old, bfd->users[i]->IV_old);
	}printf("\n");
	
	int pid = fork();

	if (pid == 0){
		current_admins++;
		if (mode == 1){ // TCP
			Administraitor_TCP(bfd);
		}else if (mode == 0){ // UDP
			Administraitor_UDP(bfd);
		}else{
			// ERROR
		}	
		return;
	}
	return;
}
