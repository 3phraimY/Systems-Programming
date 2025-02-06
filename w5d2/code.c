#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    FILE *fp;

    fp = fopen("file.txt", "r+");

    /*int ret;

    char *buf;
    buf = malloc(1000);

    char *s_ret;

    while ((s_ret = fgets(buf, 1000, fp)) != NULL)
    {
        printf("%s", buf);
    }

    free(buf); */

    void *buf;
    buf = malloc(16);
    size_t rs;

    while ((rs = fread(buf, 2, 8, fp)) > 0)
    {
        printf("%d ", (int)rs);
    }

    free(buf);
    fclose(fp);
    return 0;
}