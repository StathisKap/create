#include "../headers/Make_Git_Ignore.h"
#include "../headers/error.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void Make_Git_Ignore(char * path)
{
    char *gitingore = malloc(strlen(path) + strlen("/.gitignore") + 1);
    sprintf(gitingore,"%s/.gitignore",path);
    if(access(gitingore, F_OK) != -1)
        error(".gitignore file already exists\n", 4);
    
    FILE *ignore = fopen(gitingore, "w");
    fwrite("\n.DS_Store\n", sizeof(char), 10, ignore);
    fwrite("\n.vscode/\n", sizeof(char), 10, ignore);
    fclose(ignore);
    free(gitingore);
}