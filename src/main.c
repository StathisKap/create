#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <libgen.h>

#include "../headers/error.h"
#include "../headers/Create_Source_File.h"
#include "../headers/Launch_VScode.h"
#include "../headers/Create_Make_File.h"
#include "../headers/Git_Init.h"
#include "../headers/Make_Git_Ignore.h"
#include "../headers/Run_Python_Script.h"
#include "../headers/Create_Readme.h"

bool description_exists = false;

int main(int argc, char *argv[])
{
    //User will provide at least 1 argument
    //If they do not provide 1 argument, exit
    if (argc < 2)
        error("Usage: ./create project_path_and_name [-r ReadMe Description] \n", 1);

    //if the user has provided the flag -r then the string after as the description in the readme_file file
    char ch;
    char *description;
    while ((ch = getopt(argc, argv, "r:")) != EOF)
    {
        switch (ch)
        {
            case 'r':
                description  = malloc(1080);
                strcpy(description, optarg);
                description_exists = true;
                break;
            default:
                break;
        }
    }

    //the argument will be the name of a directory
    char *path = argv[optind];
    char *project_dirname = strrchr(path, '/') + 1;


    //create the directory if it doesn't already exist. if the directory already exists, exit
    mkdir(path, 0777) == -1 ? error("Directory already exists\n",7) : printf("Directory created: %s\n", path);
    Git_Init(path); // initialize a git repository in the project path. if the git command fails, exit
    Create_Source_File(path, project_dirname); // create a C source file with the name of the direname and the .c extension in the directory, if the file exists, exit
    Launch_VScode(path); // Open up the project directory using vscode
    Create_Make_File(path, project_dirname); // create a makefile in the directory and add a rule to compile the source file
    Make_Git_Ignore(path); // create a .gitingore file in the project path. if the file exists, exit
    Create_Readme(path, project_dirname, description); // create a README.md file in the project path with a heading that contains the name of the project
    Run_Python_Script(path, project_dirname, description); // run a python script and add git remote

    //free memory
    if (description_exists)
        free(description);
    return 0;
}
