#include "../headers/Create_Source_File.h"
#include "../headers/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void Create_Source_File(char * path, char * project_dirname)
{
    char *filename = malloc(strlen(path) + strlen(project_dirname) + 4);
    sprintf(filename,"%s/%s.c",path,project_dirname);

    if(access(filename, F_OK) != -1)
        error("Source File already exists\n", 3);

    FILE *file = fopen(filename, "w");
    fclose(file);
    printf("Source file created: %s\n", filename);
    free(filename);
}