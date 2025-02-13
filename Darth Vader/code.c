#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    FILE *pic = fopen("darthvador.bmp", "r+");
    if (pic == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    fseek(pic, 0, SEEK_END);
    int size = ftell(pic);

    // Read BMP info header
    fseek(pic, 18, SEEK_SET);
    unsigned int width, height;
    fread(&width, 4, 1, pic);
    fread(&height, 4, 1, pic);

    fseek(pic, 34, SEEK_SET);
    unsigned int size1;
    fread(&size1, 4, 1, pic);

    printf("%d\n", size1);
    printf("%d\n", width);
    printf("%d\n", height);

    // Move to first pixel
    fseek(pic, 54, SEEK_SET);

    // Create new flipped image file stream
    FILE *newPic = fopen("darthVadorFlipped.bmp", "w+");
    if (newPic == NULL)
    {
        perror("Error creating new file");
        fclose(pic);
        return 1;
    }

    // Copy file header
    void *fileHeaderBuf = malloc(54);
    fseek(pic, 0, SEEK_SET);
    fread(fileHeaderBuf, 54, 1, pic);
    fwrite(fileHeaderBuf, 54, 1, newPic);
    free(fileHeaderBuf);

    // Create a buffer for a row of the image
    char *rowBuf = malloc(width * 3);

    for (int j = 0; j < height; j++)
    {
        fread(rowBuf, 3, width, pic);
        for (int i = 0; i < width / 2; i++)
        {
            for (int k = 0; k < 3; k++)
            {
                // switch pixels
                char temp = rowBuf[i * 3 + k];
                rowBuf[i * 3 + k] = rowBuf[(width - i - 1) * 3 + k];
                rowBuf[(width - i - 1) * 3 + k] = temp;
            }
        }
        fwrite(rowBuf, 3, width, newPic);
    }

    free(rowBuf);
    fclose(newPic);
    fclose(pic);

    return 0;
}