#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>
#include "queue.h"

#define THREAD_NUM 3
#define DATA_NUM 1000000
#define MAX_QUEUE_NUM 10
#define END_DATA -1

typedef struct _thread_arg {
	int id;
	bool *primes;
	queue_t *queue;
} thread_arg_t;

queue_t queue;

void thread_func(void *arg) {
	thread_arg_t* targ = (thread_arg_t *) arg;
	int limit, i, index;

	while(1) {
		dequeue(targ->queue, &index);

		if(index == END_DATA) break;
		limit = (int)sqrt((double) index);
		for(i=2; i<=limit; i++) {
			if(targ->primes[i] && index % i == 0) {
				targ->primes[index] = false;
				break;
			}
		}
	}
	return;
}

int main() {
	pthread_t handle[THREAD_NUM];
	thread_arg_t targ[THREAD_NUM];
	bool primes[DATA_NUM];
	int i;

	queue.rp = queue.wp = queue.remain = 0;
	pthread_mutex_init(&queue.mutex, NULL);
	pthread_cond_init(&queue.not_full, NULL);
	pthread_cond_init(&queue.not_empty, NULL);
	for(i=0; i<DATA_NUM; i++) {
		primes[i] = true;
	}

	for(i=0; i<THREAD_NUM; i++) {
		targ[i].id = i;
		targ[i].primes = primes;
		targ[i].queue = &queue;
		pthread_create(&handle[i], NULL, (void*)thread_func, (void*)&targ[i]);
	}

	for(i=2; i<DATA_NUM; i++) {
		enqueue(&queue, i);
	}

	for(i=0; i<THREAD_NUM; i++) {
		enqueue(&queue, END_DATA);
	}

	for(i=0; i<THREAD_NUM; i++) {
		pthread_join(handle[i], NULL);
	}

	for(i=2; i<DATA_NUM; i++) {
		if(primes[i]) {
			printf("%d ", i);
		}
	}
	printf("\n");
	return 0;
}




