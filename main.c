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
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

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
int Check(instruction* intsr_ptr);
void Pipe(instruction* intsr_ptr);

void execute(char **cmd);
void redirOutput(char* cmd[], int Tokens);
void redirInput(char * cmd[], int Tokens);
void command(char *cmd[], int Tokens);

int main() {
	char* token = NULL;
	char* temp = NULL;
  int k;
	instruction instr;
	instr.tokens = NULL;
	instr.numTokens = 0;

	//array of background processes
	background back[10];
	for (k = 0; k < sizeof(back); ++k)
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
			int a = 0;
			int b = 0;
			if((instr.tokens)[0] == NULL)
			{
				// do nothing
			}
			else if (strcmp((instr.tokens)[0], "echo") == 0)
			{
				// call echo built in
				for (a = 1; a < instr.numTokens; a++)
				{
					//for each args after echo
					if ((instr.tokens)[a][0] == '$')
					{
						++(instr.tokens)[a]; //<- remove $ from token
						printf("%s ", getenv((instr.tokens)[a]--));
					}
					else
					{
						printf("%s ", (instr.tokens)[a]);
					}
				}
				printf("\n");
				c_count++;
			}
			else if (strcmp((instr.tokens)[0], "cd") == 0)
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
					//change directory
					int change = chdir((instr.tokens)[1]);

					if (change == -1) // 0 = pass, -1 = fail
						printf("%s: No such directory\n", (instr.tokens)[1]);
					else
						setenv("PWD", (instr.tokens)[1], 1);
				}
				c_count++;
			}
			else if (strcmp((instr.tokens)[0], "jobs") == 0)
			{
				// call jobs built in
			}
			else if (strcmp((instr.tokens)[0], "exit") == 0)
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
			else if (ExternalCommand((instr.tokens)[0]) == 1)
			{
				if(instr.numTokens == 1)
				{
					char* path = PathEnvPath((instr.tokens)[0]);
				}
				else
				{
					if (check(instr) == 1) // pipe |
					{

					}
					else if (check(instr) == 0) // i/o < >
					{

					}
					else //Not built-in and No special characters
					{
						char* path = PathEnvPath((instr.tokens)[0]);
						for(b = 0; b < instr.numTokens; b++)
						{
							if(strcmp((instr.tokens)[b], "|") == 0)
							{
								
							}
						}
					}
				}
			}
			else
			{
				printf("%s: Command Not Found.\n", (instr.token)[0]);
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

//-------------------------------------------------------------***//
void Prompt()
{
		printf("%s@%s:%s>", getenv("USER"), getenv("MACHINE"), getenv("PWD"));
}

//if | -> 1
//if < or > -> 0
//if not those -> -1
int Check(instruction* intsr_ptr)
{
	int i; //index
    for (i = 0; i < instr_ptr->numTokens; ++i)
    {
        if (strcmp(instr_ptr->tokens[i], "|") == 0)
		{
			return 1;
		}
		else if (strcmp(instr_ptr->tokens[i], "<") == 0 || strcmp(instr_ptr->tokens[i], ">") == 0)
		{
			return 0;
		}
    }
	return -1;
}

//Piping function
void Pipe(instruction* instr_ptr)
{
	//pointers to parts of command
    char **before; // < |
    char **after;  // | >

	int i; //index

	// set pointer locations
    for (i = 0; i < instr_ptr->numTokens; ++i)
    {
        if (strcmp(instr_ptr->tokens[i], "|") == 0)
        {
            //set | to NULL
            instr_ptr->tokens[i] = NULL;

            //set before to <|
            before = instr_ptr->tokens;

            //set after to |>
            after = instr_ptr->tokens + i + 1;
            break;
        }
    }

	//set up pipe
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
}

//-------------------------------------------------------------***//
//Execution Functions
void command(char *cmd[], int Tokens)
{
	int operator;
	int i = 0;
	char temp;

	for (i = 0; i < Tokens-1; i++)
	{
		//printf("%s\n", cmd[i]);
		if (strcmp(cmd[i], ">") == 0)
		{
			operator = i;
			temp = '>';
		}
		else if (strcmp(cmd[i], "<") == 0)
		{
			operator = i;
			temp = '<';
		}
		else if (strcmp(cmd[i], "|") == 0)
		{
			operator = i;
			temp = '|';
		}
		else if (strcmp(cmd[i], "&") == 0)
		{
			operator = i;
			temp = '&';
		}
	}

	if (temp == '>')
	{
		//printf("WE DOING OUTPUT REDIRECTION BABY\n");
		redirOutput(cmd, Tokens);
	}
	else if (temp == '<')
	{
		//printf("WE DOING INPUT REDIRECTION BABY\n");
		redirInput(cmd, Tokens);
	}
	else if (temp == '|')
	{

	}
	else if (temp == '&')
	{

	}
	else
	{
		execute(cmd);
	}
}


void execute(char **cmd)
{
	char bin[] = "/bin/";

	strcat(bin, cmd[0]);

	int status;
	pid_t pid = fork();

	if (pid == -1)
	{
		exit(1);
	}
	else if (pid == 0)
	{
		execv(bin, cmd);
		printf("Problem executing %s\n", bin);
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}

void redirOutput(char* cmd[], int Tokens)
{
	int i;
	int operator;

	// Figure Out Where The Farthest Right > Operator Is In Command
	for (i = 0; i < Tokens-1; i++)
	{
		if (strcmp(cmd[i], ">") == 0)
		{
			operator = i;
		}
	}

	// Checks To See If Output Redirection Has Enough Tokens
	if (operator == Tokens-1)
	{
		printf("Missing Name For Redirection.\n");
	}
	else if (Tokens < 3)
	{
		printf("Error With Output Redirection.\n");
	}
	else
	{
			char* leftCMD[256] = {0};

			// Copies All Tokens Left Of The Farthest Right > Operator
			for (i = 0; i < operator; i++)
			{
				leftCMD[i] = cmd[i];
			}


			// Attempts To Open (Or Create) Output File In Write Only Mode With User Permissions
			char fileOutputName[256];
			strcpy(fileOutputName, cmd[Tokens-2]);
			int standardOut = dup(STDOUT_FILENO);
			int file = open(fileOutputName, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);

			// Print Error If Opening Of File Failed
			if (file < 0)
			{
				printf("Error Opening Output File.\n");
				exit(1);
			}

			int status;
			pid_t pid = fork();

			if (pid == 0)
			{
				// Changes Output To Output To File
				dup2(file, STDOUT_FILENO);

				// Will Change To Go To Other Function To See What Operator 
				// Is Furhtest Left Before Execution
				command(leftCMD, i+1);

				// Closes The File And Sets Output To Default
				close(file);
				dup2(standardOut, STDOUT_FILENO);

				exit(1);
			}
			else if (pid > 0)
			{
				waitpid(pid, &status, 0);
			}
			else
			{
				printf("Output Redirection Fork Failed\n");
				exit(1);
			}
	}
}

void redirInput(char* cmd[], int Tokens)
{
	int i;
	int operator;

	// Figure Out Where The Farthest Right < Operator Is In Command
	for (i = 0; i < Tokens-1; i++)
	{
		if (strcmp(cmd[i], "<") == 0)
		{
			operator = i;
		}
	}

	// Checks To See If Output Redirection Has Enough Tokens
	if (operator == Tokens-1)
	{
		printf("Missing Name For Redirection.\n");
	}
	else if (Tokens < 3)
	{
		printf("Error With Input Redirection.\n");
	}
	else
	{
			char* leftCMD[256] = {0};

			// Copies All Tokens Left Of The Farthest Right > Operator
			for (i = 0; i < operator; i++)
			{
				leftCMD[i] = cmd[i];
			}


			// Attempts To Open Input File In Read Only Mode
			char fileInputName[256];
			strcpy(fileInputName, cmd[Tokens-2]);
			int standardOut = dup(STDOUT_FILENO);
			int file = open(fileInputName, O_RDONLY);

			// Print Error If Opening Of File Failed
			if (file < 0)
			{
				printf("Error Opening Input File.\n");
				exit(1);
			}

			int status;
			pid_t pid = fork();

			if (pid == 0)
			{
				// Changes Output To Output To File
				close(STDIN_FILENO);
				dup(file);

				// Will Change To Go To Other Function To See What Operator 
				// Is Furhtest Left Before Execution
				command(leftCMD, i+1);

				// Closes The File And Sets Output To Default
				close(file);
				dup2(standardOut, STDOUT_FILENO);

				exit(1);
			}
			else if (pid > 0)
			{
				// waitpid(pid, &status, 0);
				close(file);
			}
			else
			{
				printf("Input Redirection Fork Failed\n");
				exit(1);
			}
	}
}