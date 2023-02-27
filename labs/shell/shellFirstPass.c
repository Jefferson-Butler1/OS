#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "parse.h"

void err(char* message, int code){
	fprintf(stderr, "%s\n", message);
	exit(code);
}

int main(){
    char * line;
    while ((line = readline("> ")) != NULL){
	struct root *r = parse(line);
     	if(!r->valid){
	   fprintf(stderr, "error");
   	   continue;
	}
	if(strcmp(*r -> first_command -> argv, "exit") == 0){
    		parse_end(r);
 		free(line);
		break;
	}
	struct command *cmd = r -> first_command;
		
	while(cmd != NULL){
		printf("%s\v\n", *cmd -> argv);
		if(cmd -> next == NULL){
		//if theres only one argument
			printf("only one command, no pipe \n");
			int rc = fork();
			if(rc < 0) err("bad pipe", 1);
			else if(rc == 0){
				//child
				execvp(cmd -> argv[0], cmd ->argv);
				err("cmd not found", 1);
			}else{
				wait(NULL);
				cmd = cmd -> next;
			}

		}else{
		//if theres a pipe
			int fds [2];
    			int rc = pipe(fds);
			rc = fork();
       	    		if(rc < 0){fprintf(stderr, "bad fork"); exit(1);}
			else if (rc == 0) {
		                // child: redirect standard output to a file
				printf("child\n");
 				close(1);
       		         	dup(fds[1]);
				close(fds[0]);
               		 	close(fds[1]);

                		// now exec 
                		execvp(cmd -> argv[0], cmd -> argv); // runs word count
				fprintf(stderr, "command not found\n");
				exit(1);
			}else{
				// parent goes down this path (original process)
				rc = fork();
				cmd = cmd -> next;
				if(rc < 0) err("bad fork", 1);
				else if(rc == 0){
					//child: redirect standard input to file
                	        	close(0);
                       			dup(fds[0]);
                        		close(fds[0]);
                        		close(fds[1]);
                        		execvp(*cmd -> argv, cmd -> argv );  
					err("cmd not found", 1);
	                	}
		                else{ 
					printf("parent\n");
					wait(NULL);
				}
			}
			//fork, exec, pipe, etc
			cmd = cmd -> next;
	close(fds[0]);
	close(fds[1]);
		}
	}	
	while(wait(NULL)>0){}

    parse_end(r);
    free(line);
    }
}
