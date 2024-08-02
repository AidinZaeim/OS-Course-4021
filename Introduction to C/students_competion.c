#include <stdio.h>
#define MAX_NAME_LENGHT 50

typedef struct
{
    char name[MAX_NAME_LENGHT];
    int age;    
    int score;
}person;

typedef struct 
{
    person a;
    person b;
    float ave;

}team;

void sort(team list[], int s)
{
    int i, j;
    team temp;
    
    for (i = 0; i < s - 1; i++)
    {
        for (j = 0; j < (s - 1-i); j++)
        {
            if (list[j].ave < list[j + 1].ave)
            {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            } 
        }
    }
}

int main()
{
	int count;
    scanf("%d", &count);

    person chart[count];    
    
    for (size_t i = 0; i < count; i++)
    {    
        scanf("%s %d %d",chart[i].name, &chart[i].age, &chart[i].score);
    }
        
    if(count % 2 == 1){        
        int min_index = 0;
        for (size_t i = 0; i < count; i++)
        {
            if (chart[i].score <= chart[min_index].score)
            {
                min_index = i;
            }           
        }
        
        for (size_t i = min_index; i < count - 1; i++)
        {
            chart[i] = chart[i + 1];
        }
        count--;        
    }

    person chart2[count];

    for (size_t i = 0; i < count; i++)
        {
            chart2[i] = chart[i];
        }

    team teams[count / 2];

    for (size_t i = 0; i < count / 2; i++)
    {
        teams[i].a = chart2[i];
        teams[i].b = chart2[count-i-1];
        teams[i].ave = (chart2[i].score + chart2[count-1-i].score) / 2;
    }

    sort(teams, count / 2);

    for (size_t i = 0; i < count / 2; i++)
    {
        printf("%s %d %d, %s %d %d\n", teams[i].a.name,teams[i].a.age, teams[i].a.score, 
                                    teams[i].b.name, teams[i].b.age, teams[i].b.score);
    }
    
    
	return 0;
}
