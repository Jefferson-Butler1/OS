#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "parse.h"

void err(char* message, int code){
	fprintf(stderr, "Error: %s\n", message);
	if(code != 0) exit(code);
}

int main(){
	char * line;
	while ((line = readline("> ")) != NULL){
		struct root *r = parse(line);
	     	if(!r->valid){ err("Error: Bad Command", 0);continue;}
		struct command *cmd = r -> first_command;
		int thisI = -1, thisO = -1, nextI = -1, fds[2], rc, file;		
		while(cmd != NULL){
			if(cmd -> next != NULL){
				if(pipe(fds) < 0) err("bad pipe", 1);
				thisO = fds[1];
				nextI = fds[0];
			}else if(cmd -> outfile){
				thisO = open(cmd -> outfile, O_WRONLY | O_CREAT, 0666);
			}
			rc = fork();
			if(rc == 0){
				if(thisI != -1){close(0); dup(thisI); close(thisI);}
				if(thisO != -1){close(1); dup(thisO); close(thisO);}
				execvp(*cmd -> argv, cmd -> argv);
				err("command not found", 1);
			}
			if(thisI != -1)	close(thisI);
			if(thisO != -1)	close(thisO);
			
			cmd = cmd -> next;
			thisI = thisO = -1;
			if(nextI != -1){thisI = nextI; nextI = -1;}
			wait(NULL);
		}
    		parse_end(r);
    		free(line);
    	}
}
