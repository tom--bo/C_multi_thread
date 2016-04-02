#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_THREAD_NUM (2)
#define THREAD_NUM (5)

sem_t *sem;

void thread_func(void *arg) {
    int id = (int)arg;
    sem_wait(sem);
    printf("Thread %d started.\n", id);
    sleep(1);
    printf("Thread %d finished.\n", id);
    sem_post(sem);
}

int main() {
    pthread_t handle[THREAD_NUM];
    int i;

    // sem_init(&sem, 0, MAX_THREAD_NUM);
    sem = sem_open("/tmp/sem", O_CREAT, 0755, MAX_THREAD_NUM);
    for(i=0; i<THREAD_NUM; ++i)
        pthread_create(&handle[i], NULL, (void *)thread_func, (void *) &i);
    for(i=0; i<THREAD_NUM; ++i)
        pthread_join(handle[i], NULL);
    sem_close(sem);

    return 0;
}


