#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// count numbers from 1 to n using k threads
// result: "thread id is %ld, and my turn is %d"

const int k = 10;
const int n = 100;

pthread_t *threads;
int *count;

const int numbersPerThread = n / k;

int total = 0;

void *threadCount(void *arg)
{
    int offset = *(int *)arg;
    for (int i = offset; i < offset + numbersPerThread; i++)
    {
        // lock and add number to total
        pthread_mutex_t lock;
        // pthread_mutex_init(&lock, sum);
        pthread_mutex_lock(&lock);

        pthread_mutex_unlock(&lock);
        count[i] = i + 1;
    }
    return NULL;
}

int main()
{
    threads = malloc(k * sizeof(pthread_t));
    count = malloc(n * sizeof(int));

    int remainingNumbers = n % k;
    int offsets[k];

    for (int i = 0; i < k; i++)
    {
        offsets[i] = i * numbersPerThread;
        pthread_create(&threads[i], NULL, threadCount, &offsets[i]);
    }

    // write extra digits
    for (int i = remainingNumbers; i > 0; i--)
    {
        count[n - i] = n - i + 1;
    }

    for (int i = 0; i < k; i++)
    {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(count);

    return 0;
}
