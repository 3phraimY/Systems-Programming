#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
    int fd[2];
    pipe(fd);
    char *buff = malloc(6);
    int pid = fork();
    if (pid == 0)
    {
        printf("I am the child\n");
        read(fd[0], buff, 6);
    }
    else
    {
        printf("I am the father\n");
        write(fd[1], "hello", 6);
    }
    // printf("%s\n", buff);
    free(buff);
    return 0;
}