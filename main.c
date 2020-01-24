//COP4610
//Project 1 Starter Code
//example code for initial parsing

//*** if any problems are found with this code,
//*** please report them to the TA

#include "utility.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
	char** tokens;
	int numTokens;
} instruction;

//background process struct
typedef struct
{
	int pid;
	char* commandLine;
} background;

void addToken(instruction* instr_ptr, char* tok);
void printTokens(instruction* instr_ptr);
void clearInstruction(instruction* instr_ptr);
void addNull(instruction* instr_ptr);
void Prompt();

int main() {
	char* token = NULL;
	char* temp = NULL;

	instruction instr;
	instr.tokens = NULL;
	instr.numTokens = 0;

	//array of background processes
	background back[10];
	for (int k = 0; k < sizeof(back); ++k)
	{
		back[k].pid = 0;
		back[k].commandLine = NULL;
	}
	
	//number of instructions executed through shell
    int c_count = 0;

	while (1) {
		Prompt();
		//printf("\nPlease enter an instruction: ");

		// loop reads character sequences separated by whitespace
		do {
			//scans for next token and allocates token var to size of scanned token
			scanf("%ms", &token);
			temp = (char*)malloc((strlen(token) + 1) * sizeof(char));

			int i;
			int start = 0;
			for (i = 0; i < strlen(token); i++) {
				//pull out special characters and make them into a separate token in the instruction
				if (token[i] == '|' || token[i] == '>' || token[i] == '<' || token[i] == '&') {
					if (i-start > 0) {
						memcpy(temp, token + start, i - start);
						temp[i-start] = '\0';
						addToken(&instr, temp);
					}

					char specialChar[2];
					specialChar[0] = token[i];
					specialChar[1] = '\0';

					addToken(&instr,specialChar);

					start = i + 1;
				}
			}

			if (start < strlen(token)) {
				memcpy(temp, token + start, strlen(token) - start);
				temp[i-start] = '\0';
				addToken(&instr, temp);
			}

			//-------------------------------------------------------------***//
			//-------------------------------------------------------------***//
		
			int i = 0;
			
			if((instr.tokens)[i] == NULL)
			{
				// do nothing
			}
			else if (strmp((instr.tokens)[i], "echo") == 0)
			{
				// call echo built in
				for (int j = 1; j < instr.numTokens; j++)
				{
					//for each args after echo
					if ((instr.tokens)[j][0] == '$')
					{
						++(instr.tokens)[j]; //<- remove $ from token
						printf("%s ", getenv((instr.tokens)[j]--));
					}
					else
					{
						printf("%s ", (instr.tokens)[j]);
					}
				}
				print("\n");
				c_count++;
			}
			else if (strmp((instr.tokens)[i], "cd") == 0)
			{
				// call cd built in
				if (instr.numTokens == 1) // no args
				{
					//go to $HOME
					int change = chdir(getenv("HOME"));

					if (change == -1) // 0 = pass, -1 = fail
						printf("%s: No such directory\n", (instr.tokens)[1]);
					else
						setenv("PWD", getenv("HOME"), 1);
				}
				else
				{
					//convert args to absolute path
					//
					//
					//

					//change directory
					int change = chdir((instr.tokens)[1]);

					if (change == -1) // 0 = pass, -1 = fail
						printf("%s: No such directory\n", (instr.tokens)[1]);
					else
						setenv("PWD", (instr.tokens)[1], 1);
				}
				
			}
			else if (strmp((instr.tokens)[i], "jobs") == 0)
			{
				// call jobs built in
			}
			else if (strmp((instr.tokens)[i], "exit") == 0)
			{
				// call exit built in

				//wait for processes to finish
				//
				//while jobs array is not empty
				//	wait
				//

				printf("Exiting Now!\n");
				printf("Commands Executed: %d\n", ++c_count);

				//clean up dynamic memory
				addNull(&instr);
				clearInstruction(&instr);

				free(token);
				free(temp);

				token = NULL;
				temp = NULL;

				//terminate shell
				break;
			}
			else if (ExternalCommand((instr.tokens)[i]) == 1)
			{
				if(instr.numTokens == 1)
				{
					char* path = PathEnvPath((instr.tokens)[i]);
				}
				else{
					char* path = PathEnvPath((instr.tokens)[i]);
					
					for(int j =0 ; j < instr.numTokens; j++)
					{
						if(strmp((instr.tokens)[j], "|") == 0)
						{
							
						}
					}
				}
			}
			else
			{
				
			}

			//free and reset variables
			free(token);
			free(temp);

			token = NULL;
			temp = NULL;
		
		} while ('\n' != getchar());    //until end of line is reached

		addNull(&instr);
		clearInstruction(&instr);

	}
	
	return 0;
}

//reallocates instruction array to hold another token
//allocates for new token within instruction array
void addToken(instruction* instr_ptr, char* tok)
{
	//extend token array to accomodate an additional token
	if (instr_ptr->numTokens == 0)
		instr_ptr->tokens = (char**) malloc(sizeof(char*));
	else
		instr_ptr->tokens = (char**) realloc(instr_ptr->tokens, (instr_ptr->numTokens+1) * sizeof(char*));

	//allocate char array for new token in new slot
	instr_ptr->tokens[instr_ptr->numTokens] = (char *)malloc((strlen(tok)+1) * sizeof(char));
	strcpy(instr_ptr->tokens[instr_ptr->numTokens], tok);

	instr_ptr->numTokens++;
}

void addNull(instruction* instr_ptr)
{
	//extend token array to accomodate an additional token
	if (instr_ptr->numTokens == 0)
		instr_ptr->tokens = (char**)malloc(sizeof(char*));
	else
		instr_ptr->tokens = (char**)realloc(instr_ptr->tokens, (instr_ptr->numTokens+1) * sizeof(char*));

	instr_ptr->tokens[instr_ptr->numTokens] = (char*) NULL;
	instr_ptr->numTokens++;
}

void printTokens(instruction* instr_ptr)
{
	int i;
	printf("Tokens:\n");
	for (i = 0; i < instr_ptr->numTokens; i++) {
		if ((instr_ptr->tokens)[i] != NULL)
			printf("%s\n", (instr_ptr->tokens)[i]);
	}
}

void clearInstruction(instruction* instr_ptr)
{
	int i;
	for (i = 0; i < instr_ptr->numTokens; i++)
		free(instr_ptr->tokens[i]);

	free(instr_ptr->tokens);

	instr_ptr->tokens = NULL;
	instr_ptr->numTokens = 0;
}

void Prompt()
{
		printf("%s@%s:%s>", getenv("USER"), getenv("MACHINE"), getenv("PWD"));
}

