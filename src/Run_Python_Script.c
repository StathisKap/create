#include "../headers/Run_Python_Script.h"
#include "../headers/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <libgen.h>
#include <sys/types.h>
#include <pwd.h>


extern bool description_exists;

void Run_Python_Script(char * path, char * project_dirname, char * description)
{
    char *python_script_name_and_path;
    printf("Running Python\n");
    //run a python script and create a pipe to capture the output from the python script stdout
    //create a new process to run the python script
    #if defined(__linux__)

        //get the directory where the binary is located -- only works on Linux
        char buffer[BUFSIZ];
        if (readlink("/proc/self/exe", buffer, BUFSIZ) == -1)
            error("Error: could not get the path of the binary\nPython Script won't run. Won't create github repo\n", 9);
        else
            printf("The path of the binary is: %s\n", buffer);

        python_script_name_and_path = malloc(strlen(buffer) + strlen("/github_new_repo.py") + 1);
        sprintf(python_script_name_and_path,"%s/github_new_repo.py",dirname((buffer)));
        free(buffer);
    #else
        const char *homedir;
        if ((homedir = getenv("HOME")) == NULL)
            homedir = getpwuid(getuid())->pw_dir;

        if(access("./github_new_repo.py", F_OK) != -1){
            python_script_name_and_path = malloc(strlen("./github_new_repo.py") + 1);
            strcpy(python_script_name_and_path, dirname("./github_new_repo.py"));
            printf("The path of the python script is in the working direcotry\n");
        }
        else{
            python_script_name_and_path = malloc(strlen(homedir) +strlen("/.local/create/github_new_repo.py") + 1);
            sprintf(python_script_name_and_path,"%s/.local/create/%s",homedir,"github_new_repo.py");
        }
        if(access(python_script_name_and_path, F_OK) != -1){
            printf("The path of the python script is at %s\n",python_script_name_and_path);
            pid_t pid = fork();
            if(pid == 0)
            {
                //child process
                //create a pipe to capture the output from the python script stdout
//                int fd[2];
//                pipe(fd);
                //create a new process to run the python script
                pid_t pid = fork();
                if(pid == 0)
                {
                    //child process
                    //close the read end of the pipe
//                    close(fd[0]);
                    //dup the write end of the pipe to stdout
//                    dup2(fd[1], STDOUT_FILENO);
                    //run the python script
                    if (description_exists)
                        execlp("python3", "python3", python_script_name_and_path, project_dirname, description, NULL);
                    else
                        execlp("python3", "python3", python_script_name_and_path, project_dirname, NULL);
                }
                //parent process
                //close the write end of the pipe
//                close(fd[1]);
                //read the output from the python script
                char buf[1024];
                char remote[255];
//                while(read(fd[0], buf, 1024) > 0)
//                {
                    if (strstr(buf, "https://github.com/") != NULL)
                    {
                        strcpy(remote, buf);
                        pid = fork();
                        if(pid == 0)
                            if(execlp("git", "git", "-C", path, "remote", "add", "origin", remote, NULL))
                                error("git remote add command failed\n", 10);
                    }
//                    break;
//                }
            }
        }
#endif
}
