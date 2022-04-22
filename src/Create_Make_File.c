#include "../headers/Create_Make_File.h"
#include "../headers/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void Create_Make_File(char * path, char * project_dirname){

    char *makefile = malloc(strlen(path) + strlen("Makefile") + 1);
    strcpy(makefile, path);
    strcat(makefile, "/Makefile");

    if(access(makefile, F_OK) != -1)
        error("Makefile already exists\n", 2);
    
    FILE *make = fopen(makefile, "w");
    fprintf(make, "CC=gcc\n\n%s: %s.c\n\t$(CC) $^ -o $@\n", project_dirname, project_dirname);
    fclose(make);
    free(makefile);
}