#ifndef _PPB_QUEUE_H
#define _PPB_QUEUE_H
#include <unistd.h>
#include <pthread.h>

#ifndef MAX_QUEUE_NUM
#define MAX_QUEUE_NUM 10

#endif /* MAX_QUEUE_NUM */

typedef struct _queue {
	int values[MAX_QUEUE_NUM];
	volatile int remain;
	int rp, wp;
	pthread_mutex_t mutex;
	pthread_cond_t not_full;
	pthread_cond_t not_empty;

} queue_t;

void enqueue(queue_t* q, int v) {
	pthread_mutex_lock(&q->mutex);
	while(q->remain == MAX_QUEUE_NUM) {
		pthread_cond_wait(&q->not_full, &q->mutex);
	}
	q->values[q->wp] = v;
	q->wp++; q->remain++;
	if(q->wp == MAX_QUEUE_NUM) q->wp = 0;
	pthread_cond_signal(&q->not_empty);
	pthread_mutex_unlock(&q->mutex);
}

void dequeue(queue_t* q, int* v) {
	pthread_mutex_lock(&q->mutex);
	while(q->remain == 0){
		pthread_cond_wait(&q->not_empty, &q->mutex);
	}
	*v = q->values[q->rp];
	q->rp++; q->remain--;
	if(q->rp == MAX_QUEUE_NUM) q->rp = 0;
	pthread_cond_signal(&q->not_full);
	pthread_mutex_unlock(&q->mutex);
}

#endif /*  _PPB_QUEUE_H */



