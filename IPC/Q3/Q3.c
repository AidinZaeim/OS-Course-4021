#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>

#define MAX_VALUE 10

void generate_matrix(int* matrix, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            matrix[i * cols + j] = rand() % (MAX_VALUE + 1);
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", matrix[i * cols + j]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));

    int N = rand() % 10 + 1;
    int M = rand() % 10 + 1;
    int K = rand() % 10 + 1;

    int shm_fd_A = shm_open("/matrix_A", O_CREAT | O_RDWR, 0666);
    int shm_fd_B = shm_open("/matrix_B", O_CREAT | O_RDWR, 0666);
    int shm_fd_C = shm_open("/matrix_C", O_CREAT | O_RDWR, 0666);

    size_t shm_size_A = N * M * sizeof(int);
    size_t shm_size_B = M * K * sizeof(int);
    size_t shm_size_C = N * K * sizeof(int);

    ftruncate(shm_fd_A, shm_size_A);
    ftruncate(shm_fd_B, shm_size_B);
    ftruncate(shm_fd_C, shm_size_C);

    int* A = mmap(NULL, shm_size_A, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_A, 0);
    int* B = mmap(NULL, shm_size_B, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_B, 0);
    int* C = mmap(NULL, shm_size_C, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_C, 0);

    generate_matrix(A, N, M);
    generate_matrix(B, M, K);

    pid_t pid;
    int i;
    for (i = 0; i < N; i++) {
        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            int j, k;
            for (j = 0; j < K; j++) {
                C[i * K + j] = 0;
                for (k = 0; k < M; k++) {
                    C[i * K + j] += A[i * M + k] * B[k * K + j];
                }
            }

            exit(EXIT_SUCCESS);
        }
    }

    for (i = 0; i < N; i++) {
        wait(NULL);
    }

    printf("Matrix A:\n");
    print_matrix(A, N, M);

    printf("Matrix B:\n");
    print_matrix(B, M, K);

    printf("Matrix C:\n");
    print_matrix(C, N, K);

    munmap(A, shm_size_A);
    munmap(B, shm_size_B);
    munmap(C, shm_size_C);

    close(shm_fd_A);
    close(shm_fd_B);
    close(shm_fd_C);

    shm_unlink("/matrix_A");
    shm_unlink("/matrix_B");
    shm_unlink("/matrix_C");

    return 0;
}