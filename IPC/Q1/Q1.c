#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_PATH_SIZE 256

int main()
{    
    int pipe1[2];
    int pipe2[2];    
    int number;
    double ave = 0;
    int minimum = 32767;
    char path[MAX_PATH_SIZE];
    
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe");
        exit(EXIT_FAILURE);
    }

    printf("Enter the file path: ");
    scanf("%s", path);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("File does not exist");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Opened file successfully...\n");
    }

    while (fscanf(file, "%d", &number) == 1)
    {            
        write(pipe1[1], &number, sizeof(int));
        write(pipe2[1], &number, sizeof(int));
    }
    close(pipe1[1]);
    close(pipe2[1]);

    int pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        close(pipe1[1]);
        int num;
        while (read(pipe1[0], &num, sizeof(int)) > 0)
        {
            if (num == -1) {
                break;
            }
            if (num < minimum)
            {
                minimum = num;
            }
        }
        close(pipe1[0]);
        exit(minimum);
    }
    close(pipe1[0]);

    int pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        close(pipe2[1]);
        int num;
        int sum = 0;
        int count = 0;
        while (read(pipe2[0], &num, sizeof(int)) > 0)
        {
            if (num == -1) {
                break;
            }
            sum += num;
            count++;
        }
        close(pipe2[0]);
        if (count > 0) {
            ave = (double)sum / count;
        }
        exit(ave);
    }
    close(pipe2[0]);

    int status;
    waitpid(pid1, &status, 0);
    if (WIFEXITED(status)) {
        minimum = WEXITSTATUS(status);
    }
    waitpid(pid2, &status, 0);
    if (WIFEXITED(status)) {
        ave = (double)WEXITSTATUS(status);
    }

    printf("The minimum value of the numbers in %s is %d\n", path, minimum);
    printf("The average value of the numbers in %s is %.2f\n", path, ave);

    fclose(file);
    return 0;
}