
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char* Delete(char* str, size_t from, size_t to);

int main () {
	int i;
	char* str = "Home/COP4610/project1";
	char* newStr = Delete(str, 5, 12);
	
	printf("%s\n",newStr);
	
   return 0;
}


char* Delete(char* str, size_t from, size_t to)
{
	int size = to - from + 1;
	int length = strlen(str)+1;
	int newLength = length - size;
	char* newStr = (char*)calloc(newLength, sizeof(char));
	newStr[newLength - 1] = '\0';
	int i = 0;
	int j = 0;
	char character = str[i];
	while (character != '\0')
	{
		if (!((i >= from) && (i <= to)))
		{
			newStr[j++] = character;
		}
	character = str[++i];
	}
	str = NULL;
	return newStr;
}