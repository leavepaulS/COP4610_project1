
// COP4610 - Project 1 //
// Paul Santora        //
// echo       built-in //

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {

    // -- if command == echo -- //

    char* token = NULL;
    token = (char*)malloc(10 * sizeof(char));
    strcpy(token, "$USER");

    //for each token after echo
    //{
        if (token[0] == '$')
        {    
            ++token; //<- remove $ from token
            printf("%s ", getenv(token--)); //<- add $ back
        }
        else
            printf("%s ", token); //<- current string in token array
    //}
    printf("\n");

    free(token);

    return 0;
}