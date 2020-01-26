
// COP4610 - Project 1 //
// Paul Santora        //
// pipes             | //

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int main() {

    //example commands
    //char *cmd1[3] = {"ls", "-l", NULL};
    //char *cmd2[3] = {"wc", "-l", NULL};
    char *cmd3[6] = {"ls", "-l", "|", "wc", "-l", NULL};

    char **before;
    char **after;

    int o; //index
    for (o = 0; o < 6; o++)
    {
        if (strcmp(cmd3[o], "|") == 0)
        {
            //set | to NULL
            cmd3[o] = NULL;

            //set before to <|
            before = cmd3;

            //set after to |>
            after = cmd3 + o + 1;
            break;
        }
    }

    //set up
    int fd[2];
    pipe(fd);

    //works only with one pipe
    pid_t p = fork();
    if (p == -1)
    {
        printf("Fork Failed");
    }
    else if (p == 0) //child process
    {
        dup2(fd[0], 0);
        close(fd[1]);

        //cmd after |
        execvp(after[0], after);

        //if exec fails
        printf("exec child failed\n");
    }
    else //parent process
    {
        dup2(fd[1], 1);
        close(fd[0]);

        //cmd before |
        execvp(before[0], before);

        //if exec fails
        printf("exec parent failed\n");
    }
    return 0;
}