#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{

    int fd;
    fd = open("file.txt", O_CREAT);

    FILE *file = fopen("file.txt", "w");

    if (file == NULL)
    {
        printf("file I/O error");
    }
    else
    {
        printf("%p", file);
    }

    fclose(file);
    return 0;
}