#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>
#include "queue.h"

#define DATA_NUM 1000
#define MAX_QUEUE_NUM 10
#define END_DATA -1

typedef struct _thread_arg {
	int divider;
	queue_t *prev_q;
	queue_t *next_q;
} thread_arg_t;

queue_t *queue;

void master_func(void *arg) {
	thread_arg_t* targ = (thread_arg_t *) arg;
	int i;

	for(i=2; i<DATA_NUM; i++) {
		if(i%2!=0 || i==2) {
			enqueue(targ->next_q, i);
		}
	}
	enqueue(targ->next_q, END_DATA);
}

void thread_func(void *arg) {
	thread_arg_t* targ = (thread_arg_t *) arg;
	int i, v;

	for(i=2; i<DATA_NUM+1;i++){
		dequeue(targ->prev_q, &v);
		if(v==END_DATA) {
			enqueue(targ->next_q, v);
			break;
		}
		if(v % targ->divider != 0 || targ->divider > (int)sqrt((double)v)) {
			enqueue(targ->next_q, v);
		}
	}
	return;
}

int main() {
	int i;
	int pipeline_size;
	thread_arg_t *targ;
	pthread_t *handle;

	pipeline_size = ((int)sqrt((double)DATA_NUM) +1) /2;
	handle = (pthread_t *)malloc(pipeline_size * sizeof(pthread_t));
	targ = (thread_arg_t *) malloc(pipeline_size * sizeof(thread_arg_t));
	queue = (queue_t *)malloc(pipeline_size * sizeof(queue_t));

	for(i=0; i<pipeline_size; i++) {
		queue[i].rp =queue[i].wp = 0;
		queue[i].remain = 0;
		pthread_mutex_init(&queue[i].mutex, NULL);
		pthread_cond_init(&queue[i].not_full, NULL);
		pthread_cond_init(&queue[i].not_empty, NULL);
	}

	targ[0].divider=2;
	targ[0].next_q = &queue[0];

	pthread_create(&handle[0], NULL, (void*)master_func, (void*)&targ[0]);

	for(i=1; i<pipeline_size;i++) {
		targ[i].divider = i * 2 + 1;
		targ[i].prev_q = &queue[i-1];
		targ[i].next_q = &queue[i];
		pthread_create(&handle[i], NULL, (void*)thread_func, (void*)&targ[i]);
	}

	for(i=2; i<DATA_NUM+1; i++) {
		int v;
		dequeue(&queue[pipeline_size -1 ], &v);
		if(v==END_DATA) break;
		printf("%d ", v);
	}
	printf("\n");
	free(queue);
	free(targ);
	free(handle);
	return 0;
}




