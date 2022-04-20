#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

void main(int argc, char *argv[])
{
    //User will provide 1 argument
    //If they do not provide 1 argument, exit
    if(argc != 2)
    {
        printf("Usage: ./create <project path and name>\n");
        exit(1);
    }

    //the argument will be the name of a directory
    char *path = argv[1];
    char *dirname = strrchr(path, '/') + 1;

    //create the directory if it doesn't already exist
    //if the directory already exists, exit
    if(mkdir(path, 0777) == -1)
    {
        printf("Directory already exists\n");
        exit(1);
    }

    //create a C source file with the name of the direname and the .c extension in the directory, if the file exists, exit
    char *filename = malloc(strlen(path) + strlen(dirname) + 4);
    strcpy(filename, path);
    strcat(filename, "/");
    strcat(filename, dirname);
    strcat(filename, ".c");
    if(access(filename, F_OK) != -1)
    {
        printf("File already exists\n");
        exit(1);
    }
    FILE *file = fopen(filename, "w");
    fclose(file);


    //create a makefile in the directory and add a rule to compile the source file
    //create a makefile in the directory, if the file exists, exit
    char *makefile = malloc(strlen(path) + strlen("Makefile") + 1);
    strcpy(makefile, path);
    strcat(makefile, "/Makefile");

    if(access(makefile, F_OK) != -1)
    {
        printf("File already exists\n");
        exit(1);
    }
    FILE *make = fopen(makefile, "w");
    fprintf(make, "CC=gcc\n\n%s: %s.c\n\t$(CC) $^ -o $@\n", dirname, dirname);
    fclose(make);


    //initialize a git repository in the project path and don't use the system function,
    //use an exec function to run the git command
    //if the git command fails, exit
    char *git = malloc(strlen(path) + 5);
    strcpy(git, path);
    //create a new process to run the git command
    pid_t pid = fork();
    if(pid == 0)
    {
        //child process
        if(execlp("git", "git", "init", path, NULL))
        {
            printf("Git command failed\n");
            exit(1);
        }
    }
    

    //create a .gitingore file in the project path
    //if the file exists, exit
    char *gitingore = malloc(strlen(path) + strlen("/.gitignore") + 1);
    strcpy(gitingore, path);
    strcat(gitingore, "/.gitignore");
    if(access(gitingore, F_OK) != -1)
    {
        printf("File already exists\n");
        exit(1);
    }
    FILE *ignore = fopen(gitingore, "w");
    fclose(ignore);


    //create a README.md file in the project path with a heading that contains the name of the project
    //if the file exists, exit
    char *readme = malloc(strlen(path) + strlen("/README.md") + 1);
    strcpy(readme, path);
    strcat(readme, "/README.md");
    if(access(readme, F_OK) != -1)
    {
        printf("File already exists\n");
        exit(1);
    }
    FILE *read = fopen(readme, "w");
    fprintf(read, "# %s\n", dirname);
    fclose(read);

    //Open up the project directory using vscode
    //don't use the system function, use an exec function to run the code command
    //create a new process to run the code command
    pid = fork();
    if(pid == 0)
    {
        //child process
        if(execlp("code", "code", path, NULL))
        {
            printf("Code command failed\n");
            exit(1);
        }
    }


    //we will then print both
    printf("%s\n", dirname);
    printf("%s\n", path);

}