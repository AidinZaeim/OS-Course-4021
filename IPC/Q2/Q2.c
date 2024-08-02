#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMANDS 10

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    char* command = argv[1];

    char* commands[MAX_COMMANDS];
    int commandCount = 0;
    
    char* token = strtok(command, "|");
    while (token != NULL) {
        commands[commandCount] = token;
        commandCount++;
        token = strtok(NULL, "|");
    }
    
    int pipefd[2];
    pid_t pid;
    
    int i;
    for (i = 0; i < commandCount; i++) {
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        
        pid = fork();
        
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            if (i > 0) {
                dup2(pipefd[0], STDIN_FILENO);
            }
            
            if (i < commandCount - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
            }
            
            execlp("/bin/sh", "/bin/sh", "-c", commands[i], NULL);
            
            perror("execlp");
            exit(EXIT_FAILURE);
        }
        else {
            if (i > 0) {
                close(pipefd[0]);
            }
            
            if (i < commandCount - 1) {
                close(pipefd[1]);
            }
            
            waitpid(pid, NULL, 0);
        }
    }
    
    return 0;
}