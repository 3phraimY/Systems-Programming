#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

char intToChar(int number);

int main()
{
    FILE *genesis;
    genesis = fopen("genesis.txt", "r");
    fseek(genesis, 0, SEEK_END);
    long fileLength = ftell(genesis);

    rewind(genesis);

    int numberOfFiles = 4;
    int length = ((int)fileLength) / numberOfFiles;

    char allFileNames[numberOfFiles][20];

    for (int i = 0; i < numberOfFiles; i++)
    {
        char fileName[20] = "genesis_Part";
        char currentFileNumber = intToChar(i + 1);
        fileName[12] = currentFileNumber;

        char *newFileName = strcat(fileName, ".txt");
        strcpy(allFileNames[i], newFileName);

        int pid = fork();
        if (pid == 0)
        {
            char *buf;
            buf = malloc(length);

            fseek(genesis, i * length, SEEK_SET);

            fread(buf, length, 1, genesis);

            FILE *newFile;
            newFile = fopen(allFileNames[i], "w+");

            fwrite(buf, length, 1, newFile);
            free(buf);
            fclose(newFile);
            exit(0);
        }
    }

    FILE *newGenesisCopy;
    newGenesisCopy = fopen("newGenesisCopy.txt", "w+");

    for (int i = 0; i < numberOfFiles; i++)
    {
        wait(NULL);
        char *buf;
        buf = malloc(length);

        FILE *currentFile;
        currentFile = fopen(allFileNames[i], "r");
        fseek(currentFile, 0, SEEK_END);
        int currentFileLength = ftell(currentFile);
        rewind(currentFile);

        fread(buf, currentFileLength, 1, currentFile);
        fwrite(buf, currentFileLength, 1, newGenesisCopy);
        fclose(currentFile);
        free(buf);
    }

    fclose(genesis);
    fclose(newGenesisCopy);
    return 0;
}

char intToChar(int number)
{
    switch (number)
    {
    case 1:
        return '1';
    case 2:
        return '2';
    case 3:
        return '3';
    case 4:
        return '4';
    case 5:
        return '5';
    default:
        return '0';
    }
};