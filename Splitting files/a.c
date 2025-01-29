#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd;
    fd = open("genesis.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("file create errors");
    }

    int fd2;
    fd2 = creat("genesis_part1.txt", 0644);
    int fd3;
    fd3 = creat("genesis_part2.txt", 0644);

    char letter;
    ssize_t lettersRead;
    off_t offsetPosition;

    offsetPosition = lseek(fd, 0, SEEK_END);
    int centerPosition = offsetPosition / 2;
    lseek(fd, 0, SEEK_SET);
    int currentPosition = 0;

    while ((lettersRead = read(fd, &letter, sizeof(char))) > 0)
    {
        currentPosition++;
        if (currentPosition < centerPosition)
        {
            write(fd2, &letter, sizeof(char));
        }
        else
        {
            write(fd3, &letter, sizeof(char));
        }
    }

    close(fd);
    close(fd2);
    close(fd3);

    return 0;
}