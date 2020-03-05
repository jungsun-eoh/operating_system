
/************************************************************
* Class: CSC-415-0# Spring 2020
* Name: Jungsun Eoh
* Student ID: 918590990
* Project: SimpleShell
* File:  main.c
*
* Description: a simple shell that can handle simple linux command
*
**************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void commandLoop(void);
char* getLine(void);
char** getCmd(char* line);
void execute(char** cmdLine);


int main(int argc, char *argv[])
{
    commandLoop();

    return 0;
}

void commandLoop()
{
    char* userInput;
    char** cmd;
    int continued = 1;

    while (continued!=0)
    {
        userInput = getLine();  // get the user input in a line

        if(!strcmp(userInput, "exit")) {    exit(EXIT_SUCCESS);     }

        cmd = getCmd(userInput);    // split the user input line word by word

        execute(cmd);

        free(userInput);
        free(cmd);
    }
}
char* getLine(void)
// get user input line.
{
    int limit = 1024 * sizeof(char);
    char *buf = (char*) malloc(limit);
    int fail =1;
    int len;

    do
    {
        printf("prompt$ ");
        if (fgets(buf, limit, stdin) == NULL)
        // case: fgets() fail
        {
            printf("malloc error");
            exit(EXIT_FAILURE);
        } else if (strcmp(buf, "\n"))
        // case: user put empty line
        {   fail = 0;   }

        len = strlen(buf);
        if(buf[len-1] == '\n')
            {   buf[len-1] =0;  }

    } while(fail);
    return buf;
}
char** getCmd(char* line)
{
    int limit = 64 * sizeof(char*);
    char **cmdLine = (char*) malloc(limit);
    char *cmd;
    int index = 0;

    cmd = strtok(line, " ");
    cmdLine[index] = cmd;
    index++;

    while(cmd != NULL) {
        cmd = strtok(NULL, " ");
        if (cmd != "\n")
        {
            cmdLine[index] = cmd;
            index++;
        }
    }
    return cmdLine;
}

void execute(char** cmdLine)
{
    pid_t pid1;
    int status;

    pid1 = fork();

    if (pid1 < 0) {
        // forking error
        printf("fork failed!\n");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // child process
        execvp(cmdLine[0], cmdLine);
        exit(EXIT_FAILURE);
    } else {
        // parent process
        do {
            waitpid(pid1, &status, 0);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        printf("Child %d, exited with %d \n", pid1, status);
    }


}
