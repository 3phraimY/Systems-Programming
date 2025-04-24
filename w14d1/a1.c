#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_cond_t cond;
pthread_mutex_t lock;

void *thread_wait(void *arg)
{
    printf("let me go\n");
    pthread_cond_wait(&cond, &lock);

    printf("I am free\n");
}

int main()
{
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&lock, NULL);
    pthread_t tw;
    pthread_create(&tw, NULL, thread_wait, NULL);
    sleep(2);
    pthread_cond_signal(&cond);
    pthread_join(tw, NULL);

    return 0;
}
