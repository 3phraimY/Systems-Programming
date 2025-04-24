// create program that can read array of strings and aggregate the word frequency count

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define STRING_NUM 8

const char *str1 = "Hawthorne Road was a cricket and football ground in Bootle in Englan, It was the home ground of Bootle F.C. between 1884 and 1893.";

// create a function that can find the frequency count of words
// use a two dimensional array for storing the word frequency
// the 0 item stores the current length of the array
// the first dimension is the word and second is the frequency

struct key_freq
{
    char *key;
    int freq;
};

struct key_freq Kf[1000];

void frequency_analysis(char *str)
{
    char *buffer = malloc(50);
    int pos = 0;
    int str_length = strlen(str);

    for (int i = 0; i < str_length; i++)
    {
        if (isalpha(str[i]))
        {
            buffer[pos] = str[i];
        }
        else
        {
            buffer[pos] = '\0';
            int found = 0;
            int total = Kf[0].freq;
            int k = 1;
            for (k = 1; k <= total; k++)
            {
                if (strcmp(Kf[k].key, buffer) == 0)
                {
                    found = 1;
                    break;
                }
            }
            if (found)
            {
                Kf[k].freq++;
            }
            else
            {
                struct key_freq n;
                n.key = malloc(strlen(buffer) + 1);
                strcpy(n.key, buffer);
                Kf[total] = n;
                Kf[0].freq++;
            }
        }
    }
}