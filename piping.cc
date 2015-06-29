#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <errno.h>
#include <sys/wait.h>

using namespace std;

int main ()
{

    /* Pipe file ID's */
    int rs, pipefd[2];
    /* Parent ID's */
    int pid1, pid2;
	const int SIZE = 10, S2 = 80;
    /* User input */
    char first[S2], second[S2];
	string quit = "quit";
    /* Unix Commands */
    char *command1[SIZE], *command2[SIZE];
    
	
    /* Begin main program logic */
	memset(command1,'\0', SIZE + 1);
	 for (int i=0; i<SIZE; i++)
    {
    command1[i] = (char *)malloc(SIZE*sizeof(char*));
    command2[i] = (char *)malloc(SIZE*sizeof(char*));
    }
	
    cout << "Please enter the first command (including args.) or quit: ";
	cin.getline(first, S2);

    while (first != quit)
    {
	char * token = (char *) malloc(sizeof(char*));
	const char delimiter = ' ';
	for (unsigned i = 0; i < SIZE; i++)
	{
		if (i == 0)
		token = strtok(first, &delimiter); //token it up
		else
		token = strtok(NULL, &delimiter); //if Null hit, keep going until token = NULL
		
		if (token != NULL)
		strcpy(command1[i], token);
		else
		command1[i] = NULL;
		free(token);
	}
    cout << "Please enter the second command: ";  
    cin.getline	(second, S2);
	
	char * token2 = (char *) malloc(sizeof(char)); //allocate memory
	for (unsigned i = 0; i < SIZE; i++)
	{
		if (i == 0)
		token2 = strtok(second, &delimiter); //tokenize
		else
		token2 = strtok(NULL, &delimiter);
		
		if (token2 != NULL)
		strcpy(command2[i], token); //copy if not null
		else
		command2[i] = '\0'; 
		free(token2);
	
	}
     /* Pipe and execute user commands */

    /* Create pipe */
    rs=pipe(pipefd);
	if (rs == -1)
	{
		perror("Fail to pipe");
        exit(EXIT_FAILURE);
	}
    /* Create child processes */

    pid1 = fork();

    if (pid1 < 0)
    {
		perror("Fail to create a seoncd fork");
        exit(EXIT_FAILURE);
    }

    /* First child process */
    if (pid1 == 0)
    {
		close (1);
		dup(pipefd[1]);
		close(pipefd[0]);
		close(pipefd[1]);
		
		command1[2] = NULL;
        rs = execvp(command1[0], command1);
        
        if (rs == -1)
        {
		perror("Fail to execute second command");
        exit(EXIT_FAILURE);	
		}
    }
	
	pid2 = fork();
	
    /* Second child process */
    if (pid2 == 0)
    {
		close (0);
		dup(pipefd[0]);
		close(pipefd[0]);
		close(pipefd[1]);
		
		command1[2] = NULL;
        rs = execvp(command2[0], command2);
	
		if (rs == -1)
        {
		perror("Fail to execute second command");
        exit(EXIT_FAILURE);	
		}
    }   
    else if (pid2 == -1)
    {
		perror("Fail to create second child process");
        exit(EXIT_FAILURE);
	}

    else {/* Wait for children to terminate */
    close(pipefd[0]);
    close(pipefd[1]);
    wait(&pid1);
    wait(&pid2);
	}
    /* Repeat */
    
    cout << "Please enter the first command: ";
    cin.getline	(first, S2);
    }

    return 0;
	
}
