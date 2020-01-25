
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char* Replace(char* dest,int from, int to, char *source);

int main () {
  char* str = "Home/Xing/COP4610/project1";
	char* source = "COP111";
  char* newStr = Replace(str,5,8,source);
  printf("%s\n", newStr);  
   return 0;
}


char* Replace(char* dest,int from, int to, char *source)
{  
  // try to get the new length
	int delength = to - from +1;
	int destLen = strlen(dest);
	int sourceLen = strlen(source);
	int newLen = destLen + sourceLen - delength;
	
	char* newStr = (char*)calloc(newLen, sizeof(char));
	newStr[newLen] = '\0';
	
	int i = 0;
	int j = 0;
	int k = 0;
	int m = to+1;
	
	for(i =0; i <newLen; i++)
	{
		if(i < from)
			newStr[i] = dest[j++];
		else if ((i >= from) && (i <= from + sourceLen - 1))
			newStr[i] = source[k++];
		else 
			newStr[i] = dest[m++];
	}
	return newStr;
}

