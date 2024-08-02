#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMBER_OF_THREADS 10
#define NUMBER_OF_RESOURCES 5
#define SLEEP_TIME 5


typedef struct
{
    unsigned long resources[NUMBER_OF_RESOURCES];
    sem_t availableResources;
    pthread_mutex_t poolMutex;    
}ResourceManager;

void* PerformTask(void* args)
{
    ResourceManager* resources = (ResourceManager*)args;
    pthread_t threadID = pthread_self();
    int currentResource;
    sem_wait(&resources->availableResources);
    pthread_mutex_lock(&resources->poolMutex);    
    for (size_t i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        if(resources->resources[i] == -1)
        {            
            currentResource = i;
            resources->resources[i] = threadID;
            break;
        }        
    }

    pthread_mutex_unlock(&resources->poolMutex);

    printf("Thread %lu is performing work with resource %d\n",threadID , currentResource);
    sleep(SLEEP_TIME);
    resources->resources[currentResource] = -1;

    sem_post(&resources->availableResources);
    pthread_exit(NULL);    
}
int main()
{
    pthread_t threads[NUMBER_OF_THREADS];    
    ResourceManager* inputArgumnets = (ResourceManager*)malloc(sizeof(ResourceManager));

    //resource initialization
    for (size_t i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        inputArgumnets->resources[i] = -1;
    }
    //semaphore and mutex initialization
    sem_init(&(inputArgumnets -> availableResources), 0, NUMBER_OF_RESOURCES);
    pthread_mutex_init(&(inputArgumnets -> poolMutex), NULL);

    //thread creation
    for (size_t i = 0; i < NUMBER_OF_THREADS; i++)
    {        
        pthread_create(&threads[i], NULL, PerformTask, (void*)inputArgumnets);
    }
    //joining threads, destorying the samephore and mutex
    for (size_t i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&inputArgumnets->availableResources);
    pthread_mutex_destroy(&inputArgumnets->poolMutex);

    return 0;
}