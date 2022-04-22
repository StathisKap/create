#include "../headers/Git_Init.h"
#include "../headers/error.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void Git_Init(char * path)
{
    char *git = malloc(strlen(path) + 5);
    strcpy(git, path);

    //create a new process to run the git command
    pid_t pid = fork();
    if(pid == 0)
    {
        //child process
        if(execlp("git", "git", "init", path, NULL))
            error("Git command failed\n", 5);
    }
    free(git);
}