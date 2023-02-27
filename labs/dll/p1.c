#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    pid_t pid = getpid();
    printf("hello world (pid = %d)\n", pid);
    pid_t child = fork();
    if(child < 0){
	fprintf(stderr, "Error: couldn't fork\n");
	exit(1);
    }else if(child == 0){
	printf("I'm the child, my PID = %d(pid = %d)\n",
			getpid(), pid);
    }else{
	wait(NULL);
	printf("I'm the parent, my PID = %d(pid = %d)\n",
			getpid(), pid);
    }
    exit(0);
}
