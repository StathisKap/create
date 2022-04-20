#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <libgen.h>

void main(int argc, char *argv[])
{
    //get the directory where the binary is located -- only works on Linux
    char buffer[BUFSIZ];
    if (readlink("/proc/self/exe", buffer, BUFSIZ) == -1)
    {
        printf("Error: could not get the path of the binary\n");
        printf("Python Script won't run. Won't create github repo\n");
    }
    else
        printf("The path of the binary is: %s\n", buffer);

    //User will provide 1 argument
    //If they do not provide 1 argument, exit
    if(argc < 2)
    {
        printf("Usage: ./create project_path_and_name [-r ReadMe Description] \n");
        exit(1);
    }

    //the argument will be the name of a directory
    char *path = argv[1];
    char *project_dirname = strrchr(path, '/') + 1;

    //create the directory if it doesn't already exist
    //if the directory already exists, exit
    if(mkdir(path, 0777) == -1)
    {
        printf("Directory already exists\n");
        exit(1);
    }

    //create a C source file with the name of the direname and the .c extension in the directory, if the file exists, exit
    char *filename = malloc(strlen(path) + strlen(project_dirname) + 4);
    strcpy(filename, path);
    strcat(filename, "/");
    strcat(filename, project_dirname);
    strcat(filename, ".c");
    if(access(filename, F_OK) != -1)
    {
        printf("File already exists\n");
        exit(1);
    }
    FILE *file = fopen(filename, "w");
    fclose(file);


    //create a makefile in the directory and add a rule to compile the source file
    char *makefile = malloc(strlen(path) + strlen("Makefile") + 1);
    strcpy(makefile, path);
    strcat(makefile, "/Makefile");

    if(access(makefile, F_OK) != -1)
    {
        printf("File already exists\n");
        exit(1);
    }
    FILE *make = fopen(makefile, "w");
    fprintf(make, "CC=gcc\n\n%s: %s.c\n\t$(CC) $^ -o $@\n", project_dirname, project_dirname);
    fclose(make);

    //initialize a git repository in the project path
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
    bool description_exists = false;
    char *description = malloc(1080);
    strcpy(readme, path);
    strcat(readme, "/README.md");
    if(access(readme, F_OK) != -1)
    {
        printf("File already exists\n");
        exit(1);
    }
    FILE *readme_file = fopen(readme, "w");
    fprintf(readme_file, "# %s\n", project_dirname);
    
    //if the user has provided the flag -r then the string after as the description in the readme_file file
    char ch;
    while ((ch = getopt(argc, argv, "r:")) != EOF)
    {
        switch (ch)
        {
            case 'r':
                fprintf(readme_file, "%s\n", optarg);
                strcpy(description, optarg);
                description_exists = true;
                break;
            default:
                break;
        }
    }
    fclose(readme_file);

    //Open up the project directory using vscode
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
    printf("%s\n", project_dirname);
    printf("%s\n", path);

    //run a python script and create a pipe to capture the output from the python script stdout
    //create a new process to run the python script
    char *python_script_name_and_path = malloc(strlen(buffer) + strlen("/github_new_repo.py") + 1);
    strcpy(python_script_name_and_path, dirname(buffer));
    strcat(python_script_name_and_path, "/github_new_repo.py");

    pid = fork();
    if(pid == 0)
    {
        //child process
        //create a pipe to capture the output from the python script stdout
        int fd[2];
        pipe(fd);
        //create a new process to run the python script
        pid_t pid = fork();
        if(pid == 0)
        {
            //child process
            //close the read end of the pipe
            close(fd[0]);
            //dup the write end of the pipe to stdout
            dup2(fd[1], STDOUT_FILENO);
            //run the python script
            if (description_exists)
                execlp("python3", "python3", python_script_name_and_path, project_dirname, description, NULL);
            else
                execlp("python3", "python3", python_script_name_and_path, project_dirname, NULL);
        }
        //parent process
        //close the write end of the pipe
        close(fd[1]);
        //read the output from the python script
        char buf[1024];
        char remote[255];
        while(read(fd[0], buf, 1024) > 0)
        {
            if (strstr(buf, "https://github.com/") != NULL)
            {
                strcpy(remote, buf);
                pid = fork();
                if(pid == 0)
                {
                    //child process
                    if(execlp("git", "git", "-C", path, "remote", "add", "origin", remote, NULL))
                    {
                        printf("Git command failed\n");
                        exit(1);
                    }
                }
            }
            break;
        }
        //create a process and run a git remote add command using an exec function
    }

    //free memory
    free(path);
    free(project_dirname);
    free(description);
    free(filename);
    free(makefile);
    free(gitingore);
    free(readme);
    free(git);
}