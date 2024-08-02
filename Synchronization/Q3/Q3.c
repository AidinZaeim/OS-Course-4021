#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define MAX_PATH_LENGHT 256
#define MAX_BUFFER_SIZE 1024

char g_buffer[MAX_BUFFER_SIZE];
int eof_flag;
pthread_mutex_t buffer_mutex;

//define struct to pass it to pthreads
typedef struct 
{
    FILE* file;

}InputArguements;


//check for queue implementation in c

void read(void* args)
{        
    FILE *source = ((InputArguements*)args) -> file;
    char character;
    int read_index = 0;
    printf("read operation has started...\n");
    pthread_mutex_lock(&buffer_mutex);

    while ((character = fgetc(source)) != EOF)
    {        
        if (read_index == MAX_BUFFER_SIZE)
        {
            read_index = 0;
        }    
        if (g_buffer[read_index] != '\0')
        {   
            printf("Buffer overflow\n");            
            break;
        }
        g_buffer[read_index] = character;        
        read_index++;    
    }
    eof_flag = 1;
    printf("File read operation was successful ...\n");

    pthread_mutex_unlock(&buffer_mutex);
    pthread_exit(NULL);
}

void write(void* args)
{   
    FILE* destination = ((InputArguements*)args) -> file;
    char character;
    int index = 0;

    while (1)
    {
        if(g_buffer[index] == '\0')
        {
           if(eof_flag)
           {                
                break;
           }
            else
            {
                continue;
            }
        }
        pthread_mutex_lock(&buffer_mutex);  

        character = g_buffer[index];              
        g_buffer[index] = '\0'; 
        index++;

        pthread_mutex_unlock(&buffer_mutex);
        fputc(character, destination);
    }

    printf("file write operation was successful ...\n");  
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{    
    pthread_t writeThread;
    pthread_t readThread;
    FILE *sourceFile = fopen(argv[1], "r");    
    FILE *destinationFile = fopen(argv[2], "w");
    InputArguements readInput = {.file = sourceFile};
    InputArguements writeInput= {.file = destinationFile};    

    if (sourceFile == NULL || destinationFile == NULL)
    {
        perror("Enter the right path");
    }

    memset(g_buffer, '\0', sizeof(g_buffer));
    
    
    pthread_mutex_init(&buffer_mutex, NULL);
    pthread_create(&readThread, NULL, read, (void*)&readInput);
    pthread_create(&writeThread, NULL, write, (void*)&writeInput);
    pthread_join(readThread, NULL);
    pthread_join(writeThread, NULL);
    pthread_mutex_destroy(&buffer_mutex);
    printf("Done\n");

    return 0;
}