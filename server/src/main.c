#include "../headers/server.h"

int main(int argc, char ** argv) {

	//int stdin_copy = dup(STDIN_FILENO);
	//int stdout_copy = dup(STDOUT_FILENO);
	//close(STDIN_FILENO);
	//close(STDOUT_FILENO);

	//pid_t parpid;
	//	if((parpid=fork())<0){ 		//--здесь мы пытаемся создать дочерний процесс главного процесса (масло масляное в прямом смысле) 
	//		                   		//--точную копию исполняемой программы
	//		printf("\ncan't fork"); //--если нам по какойлибо причине это сделать не удается выходим с ошибкой.
	//		exit(1);                //--здесь, кто не совсем понял нужно обратится к man fork
	//	}
	//	else if (parpid!=0) 		//--если дочерний процесс уже существует
	//		exit(0);            		//--генерируем немедленный выход из программы(зачем нам еще одна копия программы)
	//	setsid();
	
	int mode = -1; // 0 - UDP; 1 - TCP; -1 - ERROR

	if (argc != 2){
		printf("Start program with mode: %s UDP/TCP\n", argv[0]);
		return 0;
	}
	if (strcmp("UDP", argv[1]) == 0){
		mode = 0;
	}else if (strcmp("TCP", argv[1]) == 0){
		mode = 1;
	}else{
		printf("Enter correct mode: UDP/TCP (you input: %s)\n", argv[1]);
		return 0;
	}
	log_perror("strcmp in main");

	printf("mode %d\n", mode);
	
	int code = fork();
	log_perror("fork in main");

	if (code){
		Broadcast_scanning();
	}else if (code == 0) {
		Wait_connection(mode, stdin_copy, stdout_copy);
	}else{
		log_error("fork return < 0");
		exit(EXIT_FAILURE);
	}

	return 0;
}