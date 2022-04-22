#include "../headers/Launch_VScode.h"
#include "../headers/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void Launch_VScode(char * path)
{
    pid_t pid = fork();
    if(pid == 0)
        if(execlp("code", "code", path, NULL))
            error("Launching Code failed\n", 6);
    printf("Launched VCode\n");
}