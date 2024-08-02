#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

#define SHARED_MEM_NAME "/my_shared_memory"
#define STACK_SIZE 10

typedef struct {
    int stack[STACK_SIZE];
    int top;
    sem_t mutex;
    sem_t items;
    sem_t spaces;
} SharedMemory;

void producer(int duration, SharedMemory* sharedMemory) {
    while (1) {
        sleep(duration);

        sem_wait(&sharedMemory->spaces);
        sem_wait(&sharedMemory->mutex);

        int number = rand() % 11;  // Generate random number between 0 and 10
        sharedMemory->stack[sharedMemory->top] = number;
        sharedMemory->top++;
        printf("Producer: Produced number %d\n", number);

        sem_post(&sharedMemory->mutex);
        sem_post(&sharedMemory->items);
    }
}

void consumer(SharedMemory* sharedMemory) {
    while (1) {
        sem_wait(&sharedMemory->items);
        sem_wait(&sharedMemory->mutex);

        int number = sharedMemory->stack[0];
        for (int i = 0; i < sharedMemory->top - 1; i++) {
            sharedMemory->stack[i] = sharedMemory->stack[i + 1];
        }
        sharedMemory->top--;
        printf("Consumer: Consumed number %d\n", number);

        sem_post(&sharedMemory->mutex);
        sem_post(&sharedMemory->spaces);

        // Simulating delay
        sleep(number);
    }
}

int main() {
    srand(time(NULL));  // Initialize random number generator

    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedMemory));
    SharedMemory* sharedMemory = (SharedMemory*)mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sharedMemory->top = 0;
    sem_init(&sharedMemory->mutex, 1, 1);
    sem_init(&sharedMemory->items, 1, 0);
    sem_init(&sharedMemory->spaces, 1, STACK_SIZE);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process (producer)
        producer(2, sharedMemory);
    } else {
        // Parent process (consumer)
        consumer(sharedMemory);
    }

    // Cleanup
    munmap(sharedMemory, sizeof(SharedMemory));
    shm_unlink(SHARED_MEM_NAME);

    return 0;
}