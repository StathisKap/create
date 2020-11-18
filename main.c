#include <stdio.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{
    char name[50];
    char gitinit[100];
    char textfiles[200];
    char path[] ="/home/stathis/Code/Projects/"; 
    char main[150];

    sprintf(name, "mkdir %s%s",path,argv[1]);
    sprintf(gitinit,"git init %s%s",path,argv[1]);
    sprintf(textfiles,"echo -e '.gitignore'\"\n\"'%s.c' >> %s%s/.gitignore && touch %s%s/README.md",argv[1],path,argv[1],path,argv[1]);
    sprintf(main,"code %s%s/%s.c",path,argv[1],argv[1]);
    system(name);
    system(gitinit);
    system(textfiles);
    system(main);
}