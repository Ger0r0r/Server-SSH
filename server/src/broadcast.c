#include "../headers/server.h"

SSI adm_addr;

void Broadcast_scanning(){

	struct sigaction act;
	memset(&act, 0 , sizeof(act));
	act.sa_sigaction = Update_admin_info;
	act.sa_flags = SA_SIGINFO;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);	
	act.sa_mask = set;

	sigaction(SIGUSR1, &act, 0);

	char buf[MAX_COMMAND_LENGHT] = {0};
	char buf_answer[MAX_COMMAND_LENGHT] = {0};

	int sock_fd_rcv, sock_fd_snd;
	SSI serv_addr, cli_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));

	sock_fd_rcv = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd_rcv < 0){
		perror("socket creation failed");
		exit(1);
	}

	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(BROADCAST_PORT);

	int a = 1;
	setsockopt(sock_fd_rcv, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
	bind(sock_fd_rcv, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	
	printf("START SCANNING BROADCAST\n");

	while (1) {
		socklen_t len = sizeof(cli_addr);
		recvfrom(sock_fd_rcv, buf, sizeof(buf), MSG_WAITALL,
				(struct sockaddr *) &cli_addr, &len);

		printf("Got message from broadcast, ip = %s, port = %d\nMessage : %s\n",
			inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf);

		sprintf(buf_answer, "@Your administraitor:%s:%d\n", inet_ntoa(adm_addr.sin_addr), ntohs(adm_addr.sin_port));

		sock_fd_snd = socket(AF_INET, SOCK_DGRAM, 0);
		sendto(sock_fd_snd, buf_answer, strlen(buf_answer), MSG_CONFIRM, (const struct sockaddr *) &cli_addr, sizeof cli_addr);
		memset(buf_answer, 0, sizeof buf_answer);
		memset(buf, 0, sizeof buf);
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