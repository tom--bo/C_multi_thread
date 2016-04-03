#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#define THREAD_NUM 3
#define DATA_NUM 12
#define MAX_QUEUE_NUM 5
#define THREAD_DATA_NUM (DATA_NUM/THREAD_NUM)
#define END_DATA -1

#include "queue.h"

typedef struct _thread_arg {
	int id;
	queue_t *queue;
} thread_arg_t;

void producer_func(void *arg) {
	thread_arg_t* targ = (thread_arg_t *)arg;
	int i;

	for(i=0; i<THREAD_DATA_NUM; i++) {
		int num = targ->id * THREAD_DATA_NUM + i;
		enqueue(targ->queue, num);
		printf("[Producer %d] ==> %d \n", targ->id, num);
		sleep(rand() % 3);
	}
	enqueue(targ->queue, END_DATA);
	return ;
}

void consumer_func(void *arg) {
	thread_arg_t* targ = (thread_arg_t *)arg;
	int i;

	while (1) {
		dequeue(targ->queue, &i);
		if (i == END_DATA) break;
		printf("[Consumer %d]    ==> %d \n", targ->id, i);
		sleep(rand() % 3);
	}
	return;
}

int main() {
	pthread_t producer[THREAD_NUM], consumer[THREAD_NUM];
	thread_arg_t ptarg[THREAD_NUM], ctarg[THREAD_NUM];
	queue_t queue;
	int i;

	queue.rp = queue.wp = 0;
	queue.remain = 0;
	pthread_mutex_init(&queue.mutex, NULL);
	pthread_cond_init(&queue.not_full, NULL);
	pthread_cond_init(&queue.not_empty, NULL);

	for(i=0; i<THREAD_NUM; i++) {
		ptarg[i].id = i;
		ptarg[i].queue = &queue;
		pthread_create(&producer[i], NULL, (void*)producer_func, (void*)&ptarg[i]);
	}

	for(i=0; i<THREAD_NUM; i++) {
		ctarg[i].id = i;
		ctarg[i].queue = &queue;
		pthread_create(&consumer[i], NULL, (void*)consumer_func, (void*)&ctarg[i]);
	}

	for(i=0; i<THREAD_NUM; i++){
		pthread_join(consumer[i], NULL);
	}

	return 0;
}





