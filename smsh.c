#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void printPrompt();									/* list of program functions */
char *readCommandLine();
char *parseCommand(char *cmdLine);
int isInternalCommand(char *cmd);
void executeInternalCommand(char *cmd);
void executeCmd(char *cmd);
int mkdir(const char *pathname, mode_t mode);
int chdir(const char *path);
void executeCommand(char *cmd);

char *commands[5] = {"cd", "help", "exit", "pwd", "type"};				/* list of internal commands accepted by the smsh */
int numCommands = 5;									/* number of internal commands */
char *args[3];										/* list of all arguments and options extracted from command line */
int status = 0;

int main(int argc, char **argv)
{
	while(1)
	{
		char *cmdLine;		
		char *cmd;
		pid_t pid;
		pid_t child;

		printPrompt();
		cmdLine = readCommandLine();						/* read command line input from user */
		cmd = parseCommand(cmdLine);						/* strip commands and arguments from command line */
		if(isInternalCommand(cmd))						/* check what type of command it is */
		{
			executeInternalCommand(cmd);
		}
		else
		{
			pid = fork();							/* create a child process */
			if(pid == 0) 							/* child process */
			{
				executeCommand(cmd);
			}
			else if(pid > 0) 						/* parent process */
			{
				child = waitpid(pid, &status, 0); 			/* wait for child to complete */
			}
			else
			{
				printf("Child pid: %d\n", child);
				printf("an error has ocurred\n");
			}
		}
	}
}
void executeCommand(char *cmd)								/* execute external command */
{
	char *extcommand = malloc(100);
	strcpy(extcommand, "/bin/");							/* append path to head of command to execute */
	strcat(extcommand, cmd);							/* append the command to execute to the end of the path */
	args[0] = extcommand;
	execve(extcommand, args, NULL);
}
void printPrompt()
{
	printf("SM>");									/* prompt for the smsh */
}
char *readCommandLine()
{
	char *cmdLine = malloc(100);
	fgets(cmdLine, 100, stdin);							/* get command line input from keyboard */
	cmdLine = realloc(cmdLine, 5 * sizeof(cmdLine));				/* reallocate memory */
	cmdLine[strcspn(cmdLine, "\n")] = 0;						/* strip the trailing new line character from the command line */
	return cmdLine;
}
char *parseCommand(char *cmdLine)
{
	char *cmd = strtok(cmdLine, " ");
	int i;	
	for(i = 1; i < 3; i++)
	{
		args[i] = strtok(NULL, " ");
	}				
	return cmd;
}
int isInternalCommand(char *cmd)
{
	int i;
	for(i = 0; i < numCommands; i++)
	{
		if((strcmp(commands[i], cmd)) ==0)					/* iterate over internal command list and check if user input command is internal */
		{
			return 1;							/* return 1 if true */
		}
	}
	return 0;
}
void executeInternalCommand(char *cmd)
{
	if((strcmp(commands[0], cmd)) ==0)						/* cd */
	{
		chdir(args[1]);								/* changes directory to directory specific in option to the argument */
	}
	else if((strcmp(commands[1], cmd)) ==0)						/* help */
	{
		printf("Listing of all internal commands\n");
		printf("cd	changes directory to directory specified	options		directory name\n");
		printf("help	prints a list of all internal commands		options		NA\n");
		printf("exit	closes active session and logs user out		options		NA\n");
		printf("pwd	prints current working directory		options		NA\n");
		printf("type	display information about command		options		command\n");
	}
	else if((strcmp(commands[2], cmd)) ==0)						/* exit */
	{
		exit(0);
	}
	else if((strcmp(commands[3], cmd)) ==0)						/* pwd */
	{
		char buf[100];
		printf("%s\n", getcwd(buf, 100));
	}
	else										/* type */
	{
		if((strcmp(commands[0], args[1])) ==0)					/* if argument is in the list of internal commands print shell builtin */
		{
			printf("%s is shell builtin\n", args[1]);
		}
		else if((strcmp(commands[1], args[1])) ==0)
		{
			printf("%s is shell builtin\n", args[1]);
		}
		else if((strcmp(commands[2], args[1])) ==0)
		{
			printf("%s is shell builtin\n", args[1]);
		}
		else if((strcmp(commands[3], args[1])) ==0)
		{
			printf("%s is shell builtin\n", args[1]);
		}
		else if((strcmp(commands[4], args[1])) ==0)
		{
			printf("%s is shell builtin\n", args[1]);
		}
		else
		{
			printf("%s is /usr/bin/%s\n", args[1], args[1]);		/* else print filepath because it is external */
		}
	}
}
