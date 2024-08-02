#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGHT 50

int main()
{
    int size;
    scanf("%d", &size);

    char members[size + 1][MAX_NAME_LENGHT];
    for (int i = 0; i < size; i++)
    {
        scanf("%s", members[i]);
    }

    int ins_count;
    scanf("%d", &ins_count);

    for (int i = 0; i < ins_count; i++)
    {
        char instruction[10];
        scanf("%s", instruction);

        int add_flag = strcmp(instruction, "add");
        int delete_flag = strcmp(instruction, "delete");
        int swap_flag = strcmp(instruction, "swap");

        if (add_flag == 0)
        {
            int index;
            char name1[MAX_NAME_LENGHT];
            scanf("%s", name1);
            scanf("%d", &index);

            for (int j = size; j > index; j--)
            {
                strcpy(members[j], members[j - 1]);
            }
            strcpy(members[index], name1);
            size++;
        }
        else if (delete_flag == 0)
        {
            char name1[MAX_NAME_LENGHT];
            scanf("%s", name1);
            for (int j = 0; j < size; j++)
            {
                int match_flag = strcmp(members[j], name1);
                if (match_flag == 0)
                {
                    for (int z = j; z < size - 1; z++)
                    {
                        strcpy(members[z], members[z + 1]);
                    }
                    size--;
                    break;
                }
            }
        }
        else if (swap_flag == 0)
        {
            char name1[MAX_NAME_LENGHT];
            scanf("%s", name1);

            char name2[MAX_NAME_LENGHT];
            scanf("%s", name2);

            int index1;
            int index2;
            char temp[MAX_NAME_LENGHT];

            for (int j = 0; j < size; j++)
            {
                int match_flag1 = strcmp(members[j], name1);
                int match_flag2 = strcmp(members[j], name2);

                if (match_flag1 == 0)
                {
                    index1 = j;
                }
                else if (match_flag2 == 0)
                {
                    index2 = j;
                }
            }
                                    
            strcpy(temp, members[index1]);
            strcpy(members[index1], members[index2]);
            strcpy(members[index2], temp);
            
        }
    }

    for (int i = 0; i < size; i++)
    {
        printf("%s ", members[i]);
    }

    return 0;
}