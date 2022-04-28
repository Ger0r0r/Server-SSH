#include "../headers/server.h"

int signal_input;
int current_admins = 0;
int mode;
connection * bfd; // Big Faking Database

void Wait_connection(int mmm){

	mode = mmm;

	struct sigaction act_usr1;
	memset(&act_usr1, 0 , sizeof(act_usr1));		log_perror("memset 1 in Wait connection");
	act_usr1.sa_sigaction = Send_admin_info;
	act_usr1.sa_flags = SA_SIGINFO;
	sigset_t set1;
	sigemptyset(&set1);		log_perror("sigemptyset 1 in Wait connection");
	sigaddset(&set1, SIGUSR1);			log_perror("sigaddset 1 in Wait connection");
	act_usr1.sa_mask = set1;

	struct sigaction act_usr2;
	memset(&act_usr2, 0 , sizeof(act_usr2));		log_perror("memset 2 in Wait connection");
	act_usr2.sa_sigaction = New_admin_request;
	act_usr2.sa_flags = SA_SIGINFO;
	sigset_t set2;
	sigemptyset(&set2);		log_perror("sigemptyset 2 in Wait connection");
	sigaddset(&set2, SIGUSR1);			log_perror("sigaddset 2 in Wait connection");
	act_usr2.sa_mask = set2;

	sigaction(SIGUSR1, &act_usr1, 0); // update admin addr
	log_perror("sigaction 1 in Wait connection");
	sigaction(SIGUSR2, &act_usr2, 0); // fork for new admin
	log_perror("sigaction 2 in Wait connection");

	bfd = Get_database();

	//printf("Info about connection:\n");
	//printf("Client - %s:%d\n", inet_ntoa(bfd->client.sin_addr), htons(bfd->client.sin_port));
	//printf("Keys - %s %s\n", bfd->key, bfd->iv);
	//printf("Database\n");
	//for (size_t i = 0; i < bfd->c_users; i++){
	//	printf("%s %s %s %s\n", bfd->users[i]->login, bfd->users[i]->password, bfd->users[i]->key_old, bfd->users[i]->IV_old);
	//}printf("\n");

	kill(getpid(), SIGUSR2);		log_perror("kill in Wait connection");

	while (1){}
	return;
}

void Send_admin_info(int sigN, siginfo_t* sigInfo, void* context){
	if (sigN != SIGUSR1)
		return;
	
	union sigval cn_info;
	memset(&cn_info, 0, sizeof(cn_info));		log_perror("memset in Send admin info");
	cn_info.sival_ptr = sigInfo->si_value.sival_ptr;
	sigqueue(getppid(), SIGUSR1, cn_info); 		log_perror("sigqueue in Send admin info");
	
	return;
}

void New_admin_request(int sigN, siginfo_t* sigInfo, void* context){
	if (sigN != SIGUSR2)
		return;

	//printf("Info about connection:\n");
	//printf("Client - %s:%d\n", inet_ntoa(bfd->client.sin_addr), htons(bfd->client.sin_port));
	//printf("Keys - %s %s\n", bfd->key, bfd->iv);
	//printf("Database\n");
	//for (size_t i = 0; i < bfd->c_users; i++){
	//	printf("%s %s %s %s\n", bfd->users[i]->login, bfd->users[i]->password, bfd->users[i]->key_old, bfd->users[i]->IV_old);
	//}printf("\n");
	
	int pid = fork();		log_perror("fork in New admin");
	if (pid == 0){
		current_admins++;
		if (mode == 1){ // TCP
			Administraitor_TCP(bfd);
		}else if (mode == 0){ // UDP
			Administraitor_UDP(bfd);
		}else{
			log_error("Impossible mode in New admin");
		}	
		return;
	}
	return;
}
