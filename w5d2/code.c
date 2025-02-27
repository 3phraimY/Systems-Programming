#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
    FILE *fp;
    int wordLength = 0;
    int wordCount = 0;
    char ch;

    fp = fopen("file.txt", "r");

    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == ' ')
        {
            wordCount++;
            wordLength = 0;
        }
        wordLength++;
    }

    if (wordLength > 0)
    {
        wordCount++;
    }

    fclose(fp);

    printf("The number of words in the file is: %d\n", wordCount);

    return 0;
}
