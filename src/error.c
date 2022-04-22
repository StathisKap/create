#include <../headers/error.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


void error(char * Error_Message, int Exit_Code)
{
    strerror(errno);
    printf(Error_Message);
    exit(Exit_Code);
}