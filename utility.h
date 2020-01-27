//utility function for project

#ifndef utility_H
#define utility_H
#include <stdlib.h>

// check if it is external command, return 1 = yes, 0 for no
int ExternalCommand(const char* str);
//check if it is command or not, 1 for bulitin command, 2 for external command , 0 for not command 
int Command(char* str);
//shortcut resolution
char* BuildPath(char* str);
//build absolute path with $PATH
char* PathEnvPath(char* str);
//Concatenate two string into one with slash
//char* Cat(char* first, const char* second);
//replace a string to another string form index 0 
char* Replace(char* dest, int from, int to, char* source);
//romove the dir 
char* RemoveDir(char* path);
//Delete characters from index 1 
char* Delete(char* str, size_t from, size_t to);
//return 1 if file exist and return 0 if file not exist
int FileExist(const char* filename);
//return 1 if directory and return 2 if file
int FileType(char* str);
#endif
