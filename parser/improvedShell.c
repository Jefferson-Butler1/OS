#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 64
#define MAX_PIPES 16

void execute_command(char **args, int input_fd, int output_fd) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // child process
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        if (execvp(args[0], args) < 0) {
            perror("execvp");
            exit(1);
        }
    } else {
        // parent process
        waitpid(pid, NULL, 0);
    }
}

void execute_commands(char ***commands, int num_pipes) {
    int pipes[MAX_PIPES][2];
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            exit(1);
        }
    }
    for (int i = 0; i <= num_pipes; i++) {
        char **args = commands[i];
        int input_fd = i == 0 ? STDIN_FILENO : pipes[i-1][0];
        int output_fd = i == num_pipes ? STDOUT_FILENO : pipes[i][1];
        execute_command(args, input_fd, output_fd);
    }
    for (int i = 0; i < num_pipes; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}
void execute(char **args, int num_pipes) {
    if (num_pipes > 0) {
        char ***commands = malloc(num_pipes * sizeof(char **));
        int i, j, k;
        for (i = 0, j = 0, k = 0; args[k] != NULL; k++) {
            if (strcmp(args[k], "|") == 0) {
                args[k] = NULL;
                commands[j++] = args + i;
                i = k + 1;
            }
        }
        commands[j++] = args + i;
        commands[j] = NULL;
        execute_commands(commands, num_pipes);
        free(commands);
    } else {
        execute_command(args, STDIN_FILENO, STDOUT_FILENO);
    }
}

int main() {
    char *line = NULL;
    size_t line_length = 0;
    char *args[MAX_ARGS];
    int num_pipes = 0;

    while (1) {
        printf("myshell> ");
        if (getline(&line, &line_length, stdin) == -1) {
            break;
        }
        line[strcspn(line, "\n")] = '\0'; // remove newline

        char *command = strdup(line);

        if (strcmp(command, "exit") == 0) {
            // handle exit command
            break;
        } else if (strcmp(command, "history") == 0) {
            // handle history command
            // TODO: implement command history
        } else {
            int num_args = 0;
            int input_fd = STDIN_FILENO;
            int output_fd = STDOUT_FILENO;
            char *token = strtok(command, " ");
            while (token != NULL) {
                if (strcmp(token, "<") == 0) {
                    // handle input redirection
                    token = strtok(NULL, " ");
                    input_fd = open(token, O_RDONLY);
		while (token != NULL) {
                if (strcmp(token, "<") == 0) {
                    // handle input redirection
                    token = strtok(NULL, " ");
                    input_fd = open(token, O_RDONLY);
                } else if (strcmp(token, ">") == 0) {
                    // handle output redirection
                    token = strtok(NULL, " ");
                    output_fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                } else if (strcmp(token, "|") == 0) {
                    num_pipes++;
                } else {
                    args[num_args++] = token;
                }
                token = strtok(NULL, " ");
            }
            args[num_args] = NULL;
            execute(args, num_pipes);
            num_pipes = 0;
            if (input_fd != STDIN_FILENO) {
                close(input_fd);
            }
            if (output_fd != STDOUT_FILENO) {
                close(output_fd);
            }
        }
    }
    return 0;
	}
    }
}
