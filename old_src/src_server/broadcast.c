#include "../headers/server.h"

SSI adm_addr;

void Broadcast_scanning(){

	struct sigaction act;
	memset(&act, 0 , sizeof(act));		log_perror("memset 1 in Broadcast");
	act.sa_sigaction = Update_admin_info;
	act.sa_flags = SA_SIGINFO;
	sigset_t set;
	sigemptyset(&set); 		log_perror("sigemptyset 1 in Broadcast");
	sigaddset(&set, SIGUSR1);			log_perror("sigaddset 1 in Broadcast");
	act.sa_mask = set;

	sigaction(SIGUSR1, &act, 0);		log_perror("sigaction 1 in Broadcast");

	char buf[MAX_COMMAND_LENGHT] = {0};
	char buf_answer[MAX_COMMAND_LENGHT] = {0};

	int sock_fd_rcv, sock_fd_snd;
	SSI serv_addr, cli_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));		log_perror("memset 2 in Broadcast");
	memset(&cli_addr, 0, sizeof(cli_addr));		log_perror("memset 3 in Broadcast");

	sock_fd_rcv = socket(AF_INET, SOCK_DGRAM, 0);		log_perror("socket in Broadcast");
	if(sock_fd_rcv < 0){
		log_error("socket in Broadcast_scanning");
		exit(1);
	}

	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(BROADCAST_PORT);

	int a = 1;
	setsockopt(sock_fd_rcv, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));		log_perror("setsockopt in Broadcast_scannng");
	bind(sock_fd_rcv, (struct sockaddr*) &serv_addr, sizeof(serv_addr));		log_perror("bind in Broadcast_scannng");
	

	printf("START SCANNING BROADCAST\n");

	while (1) {
		//printf("Admin %s:%d\n", inet_ntoa(adm_addr.sin_addr), ntohs(adm_addr.sin_port));
		socklen_t len = sizeof(cli_addr);
		recvfrom(sock_fd_rcv, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *) &cli_addr, &len);		log_perror("recvfrom in Broadcast_scannng");

		//printf("Got message from broadcast, ip = %s, port = %d\nMessage : %s\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf);

		sprintf(buf_answer, "%d\n", ntohs(adm_addr.sin_port));		log_perror("sprintf in Broadcast");

		sock_fd_snd = socket(AF_INET, SOCK_DGRAM, 0);		log_perror("socket in Broadcast");
		sendto(sock_fd_snd, buf_answer, strlen(buf_answer), MSG_CONFIRM, (const struct sockaddr *) &cli_addr, sizeof cli_addr);		log_perror("sendto in Broadcast_scannng");
		memset(buf_answer, 0, sizeof buf_answer);		log_perror("memset 1 in Broadcast_scannng");
		memset(buf, 0, sizeof buf);		log_perror("memser 2 in Broadcast_scannng");
	}
	close(sock_fd_rcv);
}

void Update_admin_info(int sigN, siginfo_t* sigInfo, void* context){
	if (sigN != SIGUSR1)
		return;
	
	if (context == NULL)
		return;

	size_t data = (size_t)sigInfo->si_value.sival_ptr;

	adm_addr = Translate_signal(data);
	adm_addr.sin_family = AF_INET;

	return;
}