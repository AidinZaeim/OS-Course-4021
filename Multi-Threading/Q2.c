#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/wait.h>

#define MAX_FILE_NAME_SIZE 256

int countOccurrences(const char *filename, const char *query) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    int count = 0;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *pos = buffer;

        while ((pos = strstr(pos, query)) != NULL) {
            ++count;
            pos += strlen(query);
        }
    }

    fclose(file);
    return count;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("[-] Usage: %s <file> <pattern1> <pattern2> ...\n", argv[0]);
        exit(1);
    }

    char* filename = (char*)malloc(sizeof(char) * MAX_FILE_NAME_SIZE);
    strncpy(filename, argv[1], MAX_FILE_NAME_SIZE);

    char** patterns = (char**)malloc(sizeof(char*) * (argc - 2));
    for (int i = 2; i < argc; i++) {
        patterns[i - 2] = (char*)malloc(sizeof(char) * MAX_FILE_NAME_SIZE);
        strncpy(patterns[i - 2], argv[i], MAX_FILE_NAME_SIZE);
    }

    pid_t pids[argc - 2];

    for (int i = 0; i < argc - 2; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("[-] Error in creating a new process\n");
            exit(1);
        } 
        else if (pid == 0) {
            int count = countOccurrences(filename, patterns[i]);
            printf("%s: %d\n", patterns[i], count);
            exit(count);
        } 
        else {            
            pids[i] = pid;
        }
    }

    int totalOccurrences = 0;
    for (int i = 0; i < argc - 2; i++) {
        int status;
        waitpid(pids[i], &status, 0);

        if (WIFEXITED(status)) {
            int count = WEXITSTATUS(status);
            totalOccurrences += count;
        }
    }

    printf("Total occurrences: %d\n", totalOccurrences);
    
    free(filename);
    for (int i = 0; i < argc - 2; i++) {
        free(patterns[i]);
    }
    free(patterns);

    return 0;
}