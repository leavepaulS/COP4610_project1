
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
char* Replace(char* dest, int from, int to, char* source);
char* RemoveDir(char* path);
char* Delete(char* str, size_t from, size_t to);
char* BuildPath(char* str);

int main () {
	char* str1 = "/";
	char* str2 = "~/COP4610/project1";
	char* str3 = "../project12";
	char* str4 = "./project123";
	
	char* newStr1 = BuildPath(str1);
	char* newStr2 = BuildPath(str2);
	char* newStr3 = BuildPath(str3);
	char* newStr4 = BuildPath(str4);
	
	printf("%s\n%s\n%s\n%s\n", newStr1,newStr2,newStr3,newStr4);
	
   return 0;
}

char* BuildPath(char* str)
{
	char* newStr = str;
	
	
	// check for '/'
	if(newStr[0] == '/')
	{  
		newStr = getenv("PWD");
		return newStr;
	}
	//check for ~
	if(newStr[0] == '~')
	{
		newStr = Replace(newStr, 0, 0 , getenv("Home"));	
	}

// check for ..	
	if(newStr[0] == '.' && newStr[1] == '.')
	{
		char* route = getenv("PWD");
		char* newRoute =  (char*)calloc(strlen(route)+1,sizeof(char));
		strcpy(newRoute, route);
		newRoute = RemoveDir(newStr);
		
		if (strcmp(newRoute,"")==0)
		{
			newStr = Replace(newStr, 0, strlen(newStr)-1, "/");
			free(newRoute);
			return newStr;
		}
		newStr = Replace(newStr, 0, 1, newRoute);
		free(newRoute);
	}
	
// check for . 
  if(strlen(str) == 2 && str[0] == '.' && str[1] != '.' )
	{
		// need to replace . / for root path
		if(strcmp(getenv("PWD"), "/") == 0)
			newStr = Replace(newStr, 0, 1, getenv("PWD"));
		else 
		{ // need to replace . only
			newStr = Replace(newStr, 0 , 0, getenv("PWD"));
		}
	}
	
	//check for anywhere of . and ..
	int number = 0;
	int slash = 0;
	int lastSlash = 0;
	
	while(newStr != '\0')
	{
		if((strlen(newStr) >= 2) && newStr[number-1] == '.' && newStr[number] == '/')
		{
			newStr = Delete(newStr, number - 1, number);
			number = number - 2;
		}
		else if(newStr[number] == '/')
		{
			lastSlash = slash;
			slash = number;
		}
		else if(newStr[number] == '.' && newStr[number-1] == '.')
		{
			newStr = Delete(newStr, number-2, number);
			number = number - 3;
		}
		number++;
	}
	return newStr;
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


char* RemoveDir(char* str)
{   
		int size = strlen(str)-1;
		int end = size;
		char a = str[size];
	  if(a == '/' && size == 0)
			return str;
		
	  while(a != '/')
		{
			a = str[--size];
		}
		
		return Delete(str, size, end );
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