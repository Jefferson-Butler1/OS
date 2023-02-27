#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 64
typedef struct command {
    char **argv;            ///< pointer to a simple command
    int argc;               ///< actual number of WORDs in argv
    int capacity;           ///< number of WORDs that can fit in argv
    char *outfile;          ///< if non-NULL, out redirect target
    struct command *next;   ///< if non-NULL, next command in pipeline
} command_t;

int main() {
  char input[MAX_CMD_LENGTH];
  char *args[MAX_ARGS];
  
  while (1) {
    printf("$ ");
    fflush(stdout);
    fgets(input, MAX_CMD_LENGTH, stdin);
    input[strcspn(input, "\n")] = 0; // remove trailing newline
    char* commands = (char*)malloc(MAX_CMD_LENGTH*sizeof(char));

    strcpy(commands, input); 
    char *token = strtok(input, " ");
    int tokens = 0;
    command_t *first = (command_t *) malloc(sizeof(command_t));
    command_t *cmd = first;
    first -> argc = 0;
    while (token != NULL) {
      args[tokens] = token;
      //pipes the command to the next 
      if     (strcmp(args[i], "|") == 0){
        
	token = strtok(NULL, " ");
        tokens++;
	cmd -> next -> argv[0] = token;
	cmd -> next -> argc = 1;

	cmd = cmd -> next;
      }
      else if(strcmp(args[i], ">") == 0)
      else if(strcmp(args[i], "&") == 0)
       
      cmd -> argv[cmd -> argc] = token;
      cmd -> argc ++;

      token = strtok(NULL, " ");
      tokens++;
      
    }
    args[tokens] = NULL;
    printf("input: %s\n" , commands);
    if (strcmp(args[0], "exit") == 0) {
      exit(0);
    }

    int operators [3] = {0,0,0};
    for(int i = 0; i < tokens; i++){
	if     (strcmp(args[i], "|") == 0) {
		fprintf(stderr, "Pipe Found at %d\n", i);
		operators[0] ++;
	}
	else if(strcmp(args[i], ">") == 0) {
		fprintf(stderr, "Redirect Found at %d\n", i);
		operators[1]++;
	}
	else if(strcmp(args[i], "&") == 0) {
		fprintf(stderr, "fork Found at %d\n", i);
		operators[1]++;
	}
    }
    
    int pid = fork();
    if (pid == 0) {
      execvp(args[0], args);
      printf("command not found: %s\n", args[0]);
      exit(1);
    } else {
      wait(NULL);
    }
  }
}
