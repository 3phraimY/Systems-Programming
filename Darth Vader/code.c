#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct Dimensions
{
    unsigned int width;
    unsigned int height;
};

struct Color
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

int main()
{
    FILE *pic = fopen("darthvador.bmp", "r+");

    // Read BMP info header
    fseek(pic, 18, SEEK_SET);
    struct Dimensions picDimensions;
    fread(&picDimensions.width, 4, 1, pic);
    fread(&picDimensions.height, 4, 1, pic);

    // Move to first pixel
    fseek(pic, 54, SEEK_SET);

    // Create new flipped image file stream
    FILE *newPic = fopen("darthVadorFlipped.bmp", "w+");

    // Copy file header
    void *fileHeaderBuf = malloc(54);
    fseek(pic, 0, SEEK_SET);
    fread(fileHeaderBuf, 54, 1, pic);
    fwrite(fileHeaderBuf, 54, 1, newPic);
    free(fileHeaderBuf);

    // Create a buffer for a row of the image
    struct Color *rowBuf = malloc(picDimensions.width * sizeof(struct Color));

    for (int j = 0; j < picDimensions.height; j++)
    {
        fread(rowBuf, sizeof(struct Color), picDimensions.width, pic);
        for (int i = 0; i < picDimensions.width / 2; i++)
        {
            // switch pixels
            struct Color temp = rowBuf[i];
            rowBuf[i] = rowBuf[picDimensions.width - i - 1];
            rowBuf[picDimensions.width - i - 1] = temp;
        }
        fwrite(rowBuf, sizeof(struct Color), picDimensions.width, newPic);
    }

    free(rowBuf);
    fclose(newPic);
    fclose(pic);

    return 0;
}
