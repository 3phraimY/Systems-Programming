#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int count = 0;

pthread_mutex_t lock;

void *increment(void *args)
{
    for (int i = 0; i < 100000; i++)
    {
        pthread_mutex_lock(&lock);
        count++;
        pthread_mutex_unlock(&lock);
    }
}

int main()
{
    pthread_t p1, p2;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&p1, NULL, increment, NULL);
    pthread_create(&p2, NULL, increment, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("count is %d\n", count);
    pthread_mutex_destroy(&lock);
    return 0;
}