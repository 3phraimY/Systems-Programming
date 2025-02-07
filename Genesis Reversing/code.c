#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *genesisBase;
    FILE *genesisLetterReversed;
    FILE *genesisLineReversed;

    genesisBase = fopen("genesis.txt", "r");
    genesisLetterReversed = fopen("genesisLetterReversed.txt", "w");
    genesisLineReversed = fopen("genesisLineReversed.txt", "w");

    char letterBuf;
    size_t lettersRead;
    int curOffset = 0;
    fseek(genesisBase, 0, SEEK_END);
    int fileSize = ftell(genesisBase);
    rewind(genesisBase);

    while ((lettersRead = fread(&letterBuf, sizeof(char), 1, genesisBase)) > 0)
    {
        fseek(genesisLetterReversed, fileSize - curOffset - 1, SEEK_SET);
        curOffset++;
        fputc(letterBuf, genesisLetterReversed);
    }

    char *lineBuf;
    lineBuf = malloc(100 * sizeof(char));
    rewind(genesisBase);

    while ((fgets(lineBuf, 100 * sizeof(char), genesisBase)) > 0)
    {
        lettersRead = strlen(lineBuf);
        for (int i = lettersRead - 2; i > -1; i--)
        {
            fputc(lineBuf[i], genesisLineReversed);
            if (i == 0)
            {
                fputc(lineBuf[lettersRead - 1], genesisLineReversed);
            }
        }
    }

    free(lineBuf);
    fclose(genesisBase);
    fclose(genesisLetterReversed);
    fclose(genesisLineReversed);
}