    #include <stdio.h>
    #include <stdlib.h>
    #include <dirent.h>
    #include <pthread.h>
    #include <string.h>
    #include <unistd.h>

    #define MAX_INPUT_LEN 256

    struct scan_input
    {        
        char current_dir[MAX_INPUT_LEN];
        char query[MAX_INPUT_LEN];
    };

    void* scan_dir(void* args){        
        
        struct scan_input* input = (struct scan_input*)args;
        struct dirent *entry ;
        DIR *dir = opendir(input->current_dir);
        
        if ( dir == NULL){
            pthread_exit(NULL);
        }        
        
        while ((entry = readdir(dir)) != NULL)
        {                                            
            if(strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name ,"..")==0){
                continue;
            }
                            
            char* path = (char*)malloc(sizeof(char) * MAX_INPUT_LEN);
            strcpy(path, input->current_dir);
            sprintf(input->current_dir,"%s/%s",path, entry->d_name);

            if (entry->d_type == 4) {
                pthread_t thread;
                pthread_create(&thread, NULL, scan_dir, input);                
                pthread_join(thread, NULL);
            }

            else if(strcmp(entry->d_name, input->query) == 0){
                printf("[+] Found:\n %s %s\n\n", input->current_dir, input->query);
            }

            strcpy(input->current_dir, path);
            
        }
        
        closedir(dir);
        pthread_exit(NULL);
    }

    int main(int argc,char *argv[]){

        char *QUERY = malloc(MAX_INPUT_LEN * sizeof(char));
        char *DIR = malloc(MAX_INPUT_LEN * sizeof(char));

        strcpy(DIR, "");
        strcpy(QUERY, "");

        
        struct scan_input *input = malloc(sizeof(struct scan_input));
        strcpy(input->current_dir, DIR);
        strcpy(input->query, QUERY);

        pthread_t thread;
        pthread_create(&thread, NULL, scan_dir, input);
        pthread_join(thread, NULL);
        
        free(DIR);
        free(QUERY);
        free(input);
        pthread_exit(NULL);

        return 0;
        }

