#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 64

int main() {
  char input[MAX_CMD_LENGTH];
  char *args[MAX_ARGS];
  
  while (1) {
    printf("$ ");
    fflush(stdout);
    fgets(input, MAX_CMD_LENGTH, stdin);
    input[strcspn(input, "\n")] = 0; // remove trailing newline
    
    char *token = strtok(input, " ");
    int i = 0;
    while (token != NULL) {
      args[i] = token;
      printf("%s ", args[i]);
      token = strtok(NULL, " ");
      i++;
    }
    args[i] = NULL;
    
    if (strcmp(args[0], "exit") == 0) {
      exit(0);
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
