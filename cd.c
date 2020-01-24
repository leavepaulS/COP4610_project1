
// COP4610 - Project 1 //
// Paul Santora        //
// cd         built-in //

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {

    //absolute path examples
    char* token1 = NULL;
    token1 = (char*)malloc(20 * sizeof(char));
    strcpy(token1, "/Users/Santora/Desktop/SHELL/cd.c");

    // -- if command == cd PATH -- //
    int change = chdir(token1);
    printf("Change to : %s : %d\n", token1, change);
    
    if (change == -1) // 0 = pass, -1 = fail
        printf("%s: No such directory\n", token1);
    else
        setenv("PWD", token1, 1);

    //if just cd
    printf("Change to : %s : %d\n", getenv("HOME"), chdir(getenv("HOME")));
    setenv("PWD", getenv("HOME"), 1);

    free(token1);

    return 0;
}