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
    unsigned int fileSize;
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
struct Color averageColors(struct Color color1, struct Color color2, struct Color color3, struct Color color4);

int main()
{
    FILE *pic = fopen("darthvador.bmp", "r+");

    rotate(pic, 6);

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

    FILE *temp;
    temp = fopen("temp.bmp", "w+");

    switch (mode)
    {
    case 1:
        rotate90(original, copy);
        break;
    case 2:
        rotate90(original, temp);
        rotate90(temp, copy);
        break;
    case 3:
        rotate90(original, copy);
        rotate90(copy, temp);
        rotate90(temp, copy);
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

    remove("temp.bmp");

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
    int position = ftell(file);
    fseek(file, 18, SEEK_SET);
    fread(&fileDimensions.width, 4, 1, file);
    fread(&fileDimensions.height, 4, 1, file);

    fseek(file, 0, SEEK_END);
    fileDimensions.fileSize = ftell(file);
    fseek(file, position, SEEK_SET);
    return fileDimensions;
}

void invertHeader(FILE *original, FILE *copy)
{
    int positionOriginal = ftell(original);
    int positionCopy = ftell(copy);

    void *beginningHeader;
    void *width;
    void *height;
    void *endHeader;

    beginningHeader = malloc(18);
    height = malloc(4);
    width = malloc(4);
    endHeader = malloc(28);

    fseek(original, 0, SEEK_SET);
    fseek(copy, 0, SEEK_SET);

    fseek(original, positionOriginal, SEEK_SET);
    fseek(copy, positionCopy, SEEK_SET);

    fread(beginningHeader, 18, 1, original);
    fread(height, 4, 1, original);
    fread(width, 4, 1, original);
    fread(endHeader, 28, 1, original);

    fwrite(beginningHeader, 18, 1, copy);
    fwrite(width, 4, 1, copy);
    fwrite(height, 4, 1, copy);
    fwrite(endHeader, 28, 1, copy);

    free(beginningHeader);
    free(width);
    free(height);
    free(endHeader);

    return;
}

void shrinkHeader(FILE *original, FILE *copy)
{
    int positionOriginal = ftell(original);
    int positionCopy = ftell(copy);

    void *beginningInfoHeader;
    unsigned int fileSize;
    void *endInfoHeader;
    int width;
    int height;
    void *middleInfoHeader;
    unsigned int imageSize;
    void *endHeader;

    beginningInfoHeader = malloc(2);
    endInfoHeader = malloc(12);
    middleInfoHeader = malloc(8);
    endHeader = malloc(16);

    fseek(original, 0, SEEK_SET);

    fread(beginningInfoHeader, 2, 1, original);
    fread(&fileSize, 4, 1, original);
    fread(endInfoHeader, 12, 1, original);
    fread(&width, 4, 1, original);
    fread(&height, 4, 1, original);
    fread(middleInfoHeader, 8, 1, original);
    fread(&imageSize, 4, 1, original);
    fread(endHeader, 16, 1, original);

    // shrink to 50% of original
    width = width / 2;
    height = height / 2;
    imageSize = width * height * sizeof(struct Color);
    fileSize = imageSize + 54;

    fseek(copy, 0, SEEK_SET);

    fwrite(beginningInfoHeader, 2, 1, copy);
    fwrite(&fileSize, 4, 1, copy);
    fwrite(endInfoHeader, 12, 1, copy);
    fwrite(&width, 4, 1, copy);
    fwrite(&height, 4, 1, copy);
    fwrite(middleInfoHeader, 8, 1, copy);
    fwrite(&imageSize, 4, 1, copy);
    fwrite(endHeader, 16, 1, copy);

    free(beginningInfoHeader);
    free(endInfoHeader);
    free(middleInfoHeader);
    free(endHeader);

    fseek(original, positionOriginal, SEEK_SET);
    fseek(copy, positionCopy, SEEK_SET);

    return;
}

void rotate90(FILE *original, FILE *copy)
{
    struct Dimensions originalDimensions = getDimensions(original);

    invertHeader(original, copy);

    struct Color color;

    fseek(copy, 54, SEEK_SET);

    for (unsigned int i = 0; i < originalDimensions.height; i++)
    {
        for (unsigned int j = 0; j < originalDimensions.width; j++)
        {
            fseek(original, 54 + i * originalDimensions.width * sizeof(struct Color) + j * sizeof(struct Color), SEEK_SET);
            fread(&color, sizeof(struct Color), 1, original);

            fseek(copy, 54 + j * originalDimensions.height * sizeof(struct Color) + (originalDimensions.height - i - 1) * sizeof(struct Color), SEEK_SET);
            fwrite(&color, sizeof(struct Color), 1, copy);
        }
    }

    rewind(original);
    rewind(copy);
    return;
}

void mirrorLeftRight(FILE *original, FILE *copy)
{
    struct Dimensions originalDimensions = getDimensions(original);

    // Move to first pixel
    fseek(original, 54, SEEK_SET);

    // Create a buffer for a row of the image
    struct Color *rowBuf = malloc(originalDimensions.width * sizeof(struct Color));

    for (int j = 0; j < originalDimensions.height; j++)
    {
        fread(rowBuf, sizeof(struct Color), originalDimensions.width, original);

        for (int i = 0; i < originalDimensions.width / 2; i++)
        {
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
    struct Dimensions originalDimensions = getDimensions(original);

    struct Color *rowBuf = malloc(originalDimensions.width * sizeof(struct Color));
    long rowSize = originalDimensions.width * sizeof(struct Color);

    for (int i = 0; i < originalDimensions.height; i++)
    {
        fseek(original, 54 + (originalDimensions.height - i - 1) * rowSize, SEEK_SET);
        fread(rowBuf, rowSize, 1, original);
        fwrite(rowBuf, rowSize, 1, copy);
    }

    free(rowBuf);

    return;
}

void shrink25(FILE *original, FILE *copy)
{
    struct Dimensions originalDimensions = getDimensions(original);
    long rowSize = originalDimensions.width * sizeof(struct Color);

    // Create a buffer for 1st row of the image
    struct Color *row1Buf = malloc(rowSize);
    struct Color *row2Buf = malloc(rowSize);

    if (row1Buf == NULL || row2Buf == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    // Move to first pixel
    fseek(original, 54, SEEK_SET);
    fseek(copy, 54, SEEK_SET);

    for (int i = 0; i < originalDimensions.height; i += 2)
    {
        fread(row1Buf, rowSize, 1, original);
        fread(row2Buf, rowSize, 1, original);

        for (int j = 0; j < originalDimensions.width; j += 2)
        {
            struct Color averagedColor = averageColors(row1Buf[j], row1Buf[j + 1], row2Buf[j], row2Buf[j + 1]);
            fwrite(&averagedColor, sizeof(struct Color), 1, copy);
        }
    }

    free(row1Buf);
    free(row2Buf);

    return;
}

struct Color averageColors(struct Color color1, struct Color color2, struct Color color3, struct Color color4)
{
    struct Color averagedColor;

    averagedColor.blue = (color1.blue / 4 + color2.blue / 4 + color3.blue / 4 + color4.blue / 4);
    averagedColor.green = (color1.green / 4 + color2.green / 4 + color3.green / 4 + color4.green / 4);
    averagedColor.red = (color1.red / 4 + color2.red / 4 + color3.red / 4 + color4.red / 4);

    return averagedColor;
}
