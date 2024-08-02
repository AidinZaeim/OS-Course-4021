#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void rotate(int** pixles, int n, int size){
    int** pixles2 = malloc(size * sizeof(int*));
    
    for (size_t i = 0; i < size; i++)
    {       
        pixles2[i] = malloc(size * sizeof(int));
    }

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            for (size_t z = 0; z < size; z++)
            {
                pixles2[j][z] = pixles[size - 1 - z][j];
            }                

        }
        for (size_t i = 0; i < size; i++)
        {
            for (size_t j = 0; j < size; j++)
            {
                pixles[i][j] = pixles2[i][j];
            }
            
        }
    }
}

int main()
{
    int size;
    scanf("%d", &size);
    int** pixles = malloc(size * sizeof(int*));        
    for (size_t i = 0; i < size; i++)
    {
        pixles[i] = malloc(size * sizeof(int));        
    }    
    
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            scanf("%d", &pixles[i][j]);            
        }
    }    

    if (pixles[0][size - 1] == 0)
    {
        rotate(pixles, 3, size);
    }
    else if (pixles[size - 1][size -1] == 0)
    {
        rotate(pixles, 2, size);
    }
    else if (pixles[size - 1][0] == 0)
    {
        rotate(pixles, 1, size);
    }

    for (size_t i = 0; i < size; i++)
    {
        printf("\n");
        for (size_t j = 0; j < size; j++)
        {
            printf("%d ", pixles[i][j]);
        }
    }
    
}