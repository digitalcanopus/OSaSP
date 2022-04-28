#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>
#include <errno.h>

void output(){
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	struct tm* time = localtime(&ts.tv_sec);
	printf("pid: %d\tparent pid: %d\ttime: %d:%d:%d:%d \n", getpid(), getppid(), time->tm_hour, time->tm_min, time->tm_sec, (int)ts.tv_nsec/100000);	
}

int main(){
	printf("parent process: ");
	output();
	pid_t pid;
	for (int i = 0; i < 2; i++){
		pid = fork();
		if (pid == 0){			
			printf("child process %d: ", i + 1);
			output();
			return 0;
		}
		else if (pid < 0){
			fprintf(stderr, "creating child process error.");
			return 1;
		}
	}

	system("ps -x"); //active user's processes

	while (1){
        	if (wait(NULL) == -1){
            		if (errno == ECHILD)
                		break;
            		fprintf(stderr, "error occurred while waiting for child process to close.");
        	}
    	}
	return 0;
}
