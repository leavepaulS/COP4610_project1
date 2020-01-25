
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int FileType(char* str);
int FileExist(const char* filename);

int main () {
  const char* firstFile = "background.c";
	const char* secondFile = "back.c";
	char* third = "background.c";
	char* forth = "COP4610";
	
	int a = FileExist(firstFile);
	int b = FileExist(secondFile);
	int c = FileType(third);
	int d = FileType(forth);
	
	if(a == 1)
		printf("Exist\n");
	else 
		printf("No such a file\n");
	
	if(b == 1)
		printf("Exist\n");
	else 
		printf("No such a file\n");
	
	if(c == 1)
		printf("Directory\n");
	else if(c == 2)
		printf("It is a file\n");
	
	if(d == 1)
		printf("Directory\n");
	else if(d == 2)
		printf("It is a file\n");
	
	
   return 0;
}


int FileExist(const char* filename)
{
	// file exist
	if(access(filename, F_OK) != -1)
		  return 1;
	// file not exist
	else 
			return 0;
}

int FileType(char* str)
{
	struct stat s;
  if( stat(str,&s) == 0 )
  {
    if( s.st_mode & S_IFDIR )
    {
        return 1;
    }
    else if( s.st_mode & S_IFREG )
    {
        return 2;
    }
 }
}

