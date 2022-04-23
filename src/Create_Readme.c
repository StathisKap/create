#include "../headers/Create_Readme.h"
#include "../headers/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

extern bool description_exists;

void Create_Readme(char *path, char *project_dirname, char * description){
    char *readme = malloc(strlen(path) + strlen("/README.md") + 1);
    sprintf(readme,"%s/README.md",path);
    if(access(readme, F_OK) != -1)
        error("README.md File already exists\n", 8);
    FILE *readme_file = fopen(readme, "w");
    fprintf(readme_file, "# %s\n", project_dirname);
    if (description_exists)
        fprintf(readme_file, "%s\n", description);
    printf("README.md file created. Title of project: %s\n", project_dirname);
    
    fclose(readme_file);
    free(readme);
}