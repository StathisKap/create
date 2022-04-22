#include "../headers/Run_Python_Script.h"
#include "../headers/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <libgen.h>

extern bool description_exists;

void Run_Python_Script(char * path, char * project_dirname, char * description)
{
    //run a python script and create a pipe to capture the output from the python script stdout
    //create a new process to run the python script
    #if defined(__linux__)

        //get the directory where the binary is located -- only works on Linux
        char buffer[BUFSIZ];
        if (readlink("/proc/self/exe", buffer, BUFSIZ) == -1)
            error("Error: could not get the path of the binary\nPython Script won't run. Won't create github repo\n", 9);
        else
            printf("The path of the binary is: %s\n", buffer);
    
        char *python_script_name_and_path = malloc(strlen(buffer) + strlen("/github_new_repo.py") + 1);
        sprintf(python_script_name_and_path,"%s/github_new_repo.py",dirname((buffer)));
        free(buffer);
    #else
        if(access("./github_new_repo.py", F_OK) != -1)
        {
            char *python_script_name_and_path = malloc(strlen("./github_new_repo.py") + 1);
            strcpy(python_script_name_and_path, dirname("./github_new_repo.py"));
            pid_t pid = fork();
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
                                error("git remote add command failed\n", 10);
                        }
                    }
                    break;
                }
            }
            free(python_script_name_and_path);
        }
        else
            printf("Python script is not on the current working directory\n");
    #endif
}