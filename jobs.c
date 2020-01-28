
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "utility.c"

typedef struct
{
	char* inputCommand;
	pid_t pidNum;
    int curStatus;
    int index;
}background;

void Jobs(background back_[], int b_size);

int main()
{
    background Back[100];
    int b_size = 0;
    Back[0].inputCommand = NULL;
    Back[0].pidNum = 0;
    Back[0].curStatus = 1;
    Back[0].index = 1;

    char temp[] = {"sleep 5 &"};
    
    Back[0].inputCommand = temp;
    Back[0].pidNum = getpid();
    Back[0].curStatus = 0;
    b_size++;

    Jobs(Back, b_size);

    return 0;
}

void Jobs(background back_[], int b_size)
{
    int b;
    //print current background commands
    for (b = 0; b < b_size; b++)
    {
        if (back_[b].curStatus == 0)
        {
            printf("[%d]+ %d   %s\n", back_[b].index, back_[b].pidNum, back_[b].inputCommand);
        }
    }
}