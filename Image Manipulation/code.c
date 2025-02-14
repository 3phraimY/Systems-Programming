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

// Prototypes
void rotate(FILE *original, int mode);
void copyHeader(FILE *original, FILE *copy);
struct Dimensions getDimensions(FILE *file);
void invertHeader(FILE *original, FILE *copy);
void shrinkHeader(FILE *original, FILE *copy);
void rotate90(FILE *original, FILE *copy);
void mirrorLeftRight(FILE *original, FILE *copy);
void mirrorTopBottom(FILE *original, FILE *copy);
void shrink25(FILE *original, FILE *copy);

int main()
{
    FILE *pic = fopen("darthvador.bmp", "r+");

    rotate(pic, 4);

    fclose(pic);

    return 0;
}

// modes
//  1 -> 90  2 -> -> 180 3 -> 270 4 -> mirror left/right 5 -> mirror top/bottom 6 -> shrink to 25% of original
void rotate(FILE *original, int mode)
{
    // name files based on mode
    char *fileNameArray[] = {"90_degrees.bmp", "180_degrees.bmp", "270_degrees.bmp", "mirror_left_right.bmp", "mirror_top_bottom.bmp", "shrunk.bmp"};

    FILE *copy;
    copy = fopen(fileNameArray[mode - 1], "w+");

    switch (mode)
    {
    case 1:
        rotate90(original, copy);
        break;
    case 2:
        copyHeader(original, copy);
        rotate90(original, copy);
        rotate90(original, copy);
        break;
    case 3:
        rotate90(original, copy);
        rotate90(original, copy);
        rotate90(original, copy);
        break;
    case 4:
        copyHeader(original, copy);
        mirrorLeftRight(original, copy);
        break;
    case 5:
        copyHeader(original, copy);
        mirrorTopBottom(original, copy);
        break;
    case 6:
        shrinkHeader(original, copy);
        shrink25(original, copy);
        break;
    }
    fclose(copy);

    return;
}

void copyHeader(FILE *original, FILE *copy)
{
    void *fileHeaderBuf = malloc(54);
    fseek(original, 0, SEEK_SET);
    fread(fileHeaderBuf, 54, 1, original);
    fwrite(fileHeaderBuf, 54, 1, copy);
    free(fileHeaderBuf);

    return;
}

struct Dimensions getDimensions(FILE *file)
{
    struct Dimensions fileDimensions;
    fseek(file, 18, SEEK_SET);
    fread(&fileDimensions.width, 4, 1, file);
    fread(&fileDimensions.height, 4, 1, file);

    return fileDimensions;
}

void invertHeader(FILE *original, FILE *copy)
{
    return;
}

void shrinkHeader(FILE *original, FILE *copy)
{
    return;
}

void rotate90(FILE *original, FILE *copy)
{
    return;
}

void mirrorLeftRight(FILE *original, FILE *copy)
{
    // Move to first pixel
    fseek(original, 54, SEEK_SET);
    fseek(copy, 54, SEEK_SET);

    struct Dimensions originalDimensions = getDimensions(original);

    // Create a buffer for a row of the image
    struct Color *rowBuf = malloc(originalDimensions.width * sizeof(struct Color));

    for (int j = 0; j < originalDimensions.height; j++)
    {
        fread(rowBuf, sizeof(struct Color), originalDimensions.width, original);

        for (int i = 0; i < originalDimensions.width / 2; i++)
        {
            // switch pixels
            struct Color colorTemp = rowBuf[i];
            rowBuf[i] = rowBuf[originalDimensions.width - i - 1];
            rowBuf[originalDimensions.width - i - 1] = colorTemp;
        }

        fwrite(rowBuf, sizeof(struct Color), originalDimensions.width, copy);
    }

    free(rowBuf);
    return;
}

void mirrorTopBottom(FILE *original, FILE *copy)
{
    return;
}

void shrink25(FILE *original, FILE *copy)
{
    return;
}
