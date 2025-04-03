#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// count numbers from 1 to n using k threads
// result: "thread id is %ld, and my turn is %d"

const int k = 10;
const int n = 101;

pthread_t *threads;
int *count;

const int numbersPerThread = n / k;

void *threadCount(void *arg)
{
    int offset = *(int *)arg;
    for (int i = offset; i < offset + numbersPerThread; i++)
    {
        count[i] = i + 1;
    }
    return NULL;
}

int main()
{
    threads = malloc(k * sizeof(pthread_t));
    count = malloc(n * sizeof(int));

    int remainingNumbers = n % k;

    for (int i = 0; i < k; i++)
    {
        int offset = i * numbersPerThread;
        pthread_create(&threads[i], NULL, threadCount, &offset);
    }

    for (int i = 0; i < k; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // write extra digits
    for (int i = remainingNumbers; i > 0; i--)
    {
        count[n - i] = n - i + 1;
    }

    // Print the result
    for (int i = 0; i < n; i++)
    {
        printf("%d\n", count[i]);
    }

    free(threads);
    free(count);

    return 0;
}
