#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define THREAD_NUM 2
#define DATA_NUM 10

typedef struct _thread_arg {
    int thread_no;
    int *data;
} thread_arg_t;

void thread_func(void *arg) {
    thread_arg_t* targ = (thread_arg_t *) arg;
    int i;
    for(i=0; i<DATA_NUM; i++) {
        printf("thread%d : %d + 1 = %d\n", targ->thread_no, targ->data[i], targ->data[i] + 1);
    }
}

int main() {
    pthread_t handle[THREAD_NUM];
    thread_arg_t targ[THREAD_NUM];
    int data[DATA_NUM];
    int i;

    for(i=0; i<DATA_NUM; i++) data[i] = i;
    for(i=0; i<THREAD_NUM; i++) {
        targ[i].thread_no = i;
        targ[i].data = data;

        pthread_create(&handle[i], NULL, (void *)thread_func, (void *)&targ[i]);
    }

    for(i=0; i<THREAD_NUM; i++) {
        pthread_join(handle[i], NULL);
    }
    return 0;
}


