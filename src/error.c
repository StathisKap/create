#include "../headers/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


void error(char * Error_Message, int Exit_Code)
{
    strerror(errno);
    puts(Error_Message);
    exit(Exit_Code);
}