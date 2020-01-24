#include "utility.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int ExternalCommand(const char* str)
{
	//build a absolute path based on str
	char* newStr = (char*)calloc(strlen(str)+2, sizeof(char));
	//separate $PATH into substr
	char* path = getenv("PATH");
	char* NewPath = (char*)calloc(strlen(path)+1, sizeof(char));
	strcpy(newStr,"/");
	strcat(newStr,str);
	
	strcpy(NewPath, path);
	//separated by :
	char* token = strtok(NewPath, ":");
	
	while(token!= NULL)
	{
		
		char* newOne = (char*)calloc(strlen(token)+1, sizeof(char));
		strcpy(newOne, token);
		int length = strlen(newOne) -1;
		
		// make sure / is not the last character
		if (newOne[length] == '/')
		{
			str = Delete(newOne, length, length);
		}
		
		strcat(newOne,newStr);
		newOne = BuildPath(newOne);
		// if it is file return 1
		int exist = FileType(newOne);
		if(exist = 1)
		{
			free(newStr);
			free(NewPath);
			return 1;
		}
	  free(newOne);
		token = strtok(NULL, ":");
	}
	// if no file exist, just return the str
	free(newStr);
	free(NewPath);
	return 0;
}

int Command(char* str)
{
	int External = ExternalCommand(str);
	if(strcmp(str	,"exit") == 0)
		return 1;
	else if (strcmp(str	,"cd") == 0)
		return 1 ;
	else if (strcmp(str	,"echo") == 0)
		return 1;
	else if(strcmp(str ,"jobs") == 0)
		return 1;
	
	if(External = 1)
		return 2;
	
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

char* PathEnvPath(char* str)
{
	//build a absolute path based on str
	char* newStr = (char*)calloc(strlen(str)+2, sizeof(char));
	//separate $PATH into substr
	char* path = getenv("PATH");
	char* NewPath = (char*)calloc(strlen(path)+1, sizeof(char));
	strcpy(newStr,"/");
	strcat(newStr,str);
	
	strcpy(NewPath, path);
	//separated by :
	char* token = strtok(NewPath, ":");
	
	while(token!= NULL)
	{
		
		char* newOne = (char*)calloc(strlen(token)+1, sizeof(char));
		strcpy(newOne, token);
		int length = strlen(newOne) -1;
		
		// make sure / is not the last character
		if (newOne[length] == '/')
		{
			str = Delete(newOne, length, length);
		}
		
		strcat(newOne,newStr);
		newOne = BuildPath(newOne);
		// if it is file return 1
		int exist = FileType(newOne);
		if(exist = 1)
		{
			free(newStr);
			free(NewPath);
			return newOne;
		}
	  free(newOne);
		token = strtok(NULL, ":");
	}
	// if no file exist, just return the str
	free(newStr);
	free(NewPath);
	return str;
	
}

/*
char* Cat(char* first, const char* second)
{
	int length = strlen()
	char* newStr;
  strcpy(first, "/");		
}
*/
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
			newStr[j] = character;
		}
	character = newStr[++i];
	}
	str = NULL;
	return newStr;
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

