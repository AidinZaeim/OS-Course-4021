#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem_one;
sem_t sem_two;
sem_t sem_three;

void* p1(void* arg){
    sem_wait(&sem_two);

    printf("F\n");

    sem_post(&sem_one);
    
    printf("E\n");
    printf("G\n");

    pthread_exit(NULL);
}

void* p2(void* arg){
    printf("A\n");

    sem_post(&sem_two);
    sem_wait(&sem_one);
    
    printf("C\n");

    sem_post(&sem_three);

    printf("B\n");

    pthread_exit(NULL);
}

int main(){
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 0);
    sem_init(&sem_three, 0, 0);

    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, p1, NULL);
    pthread_create(&thread2, NULL, p2, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    sem_destroy(&sem_three);

    return 0;
}
