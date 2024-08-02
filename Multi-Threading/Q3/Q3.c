#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int rows;
    int cols;
    printf("Enter the number of rows of the original array\n");
    scanf("%d", &rows);
    printf("Enter the number of columns of the original array\n");
    scanf("%d", &cols);

    int** mat = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
        mat[i] = (int*)malloc(cols * sizeof(int));

    printf("Enter the matrix elements\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &mat[i][j]);
        }
    }

    int w_rows;
    int w_cols;
    printf("Enter the number of rows of the window array\n");
    scanf("%d", &w_rows);
    printf("Enter the number of columns of the window array\n");
    scanf("%d", &w_cols);

    int** window = (int**)malloc(w_rows * sizeof(int*));
    for (int i = 0; i < w_rows; i++)
        window[i] = (int*)malloc(w_cols * sizeof(int));

    printf("Enter the window matrix elements\n");
    for (int i = 0; i < w_rows; i++) {
        for (int j = 0; j < w_cols; j++) {
            scanf("%d", &window[i][j]);
        }
    }

    clock_t start_t, end_t;
    double total_t;
    start_t = clock(); 

    int** result = (int**)malloc((rows - w_rows + 1) * sizeof(int*));
    for (int i = 0; i < (rows - w_rows + 1); i++) {
        result[i] = (int*)malloc((cols - w_cols + 1) * sizeof(int));
    }

    int i, j;
    pid_t child_pid;
    int child_count = 0;

    for (i = 0; i < (rows - w_rows + 1); i++) {
        for (j = 0; j < (cols - w_cols + 1); j++) {
            child_pid = vfork();

            if (child_pid < 0) {
                perror("Fork failed");
                return 1;
            }

            if (child_pid == 0) {
                int sum = 0;
                for (int k = 0; k < w_rows; k++) {
                    for (int l = 0; l < w_cols; l++) {
                        sum += mat[i + k][j + l] * window[k][l];
                    }
                }
                result[i][j] = sum;
                exit(0);
            }
            else {
                child_count++;
            }
        }
    }
    
    for (i = 0; i < child_count; i++) {
        waitpid(-1, NULL, 0);
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken: %f\n", total_t);    

    for (int i = 0; i < rows; i++) {
        free(mat[i]);
    }
    for (int i = 0; i < w_rows; i++) {
        free(window[i]);
    }
    for (int i = 0; i < (rows - w_rows + 1); i++) {
        free(result[i]);
    }
    free(mat);
    free(window);
    free(result);

    return 0;
}
