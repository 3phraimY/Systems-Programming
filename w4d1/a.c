#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd, fd2;
    fd = open("file.txt", O_RDONLY);
    fd2 = creat("file2.txt", 0644);

    ssize_t ret; // sucessful size of read
    char arr[8];

    char *buf;

    while ((ret = read(fd, arr, 8 * sizeof(char))) > 0)
    {
        write(fd2, arr, ret);
    }
}