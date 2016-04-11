#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>

#include "timer.h"

#define THREAD_NUM 5
#define DATA_NUM 1000000

typedef struct _thread_arg {
	int id;
	bool *primes;
} thread_arg_t;

void thread_func(void *arg) {
	thread_arg_t* targ = (thread_arg_t *) arg;
	int c_start, c_end, range, limit;
	int i,j;

	range = (DATA_NUM -2) / THREAD_NUM + 1;
	c_start = 2 + targ->id * range;
	c_end = 2 + (targ->id + 1) * range;
	if(c_end > DATA_NUM) c_end = DATA_NUM;

	for(i = c_start; i < c_end; i++) {
		limit = (int) sqrt((double) i);
		for(j=2; j<= limit; j++) {
			if(targ->primes[j] && i%j==0) {
				targ->primes[i] = false;
				break;
			}
		}
	}
	return;
}

int main() {
	unsigned int t, time;
	start_timer(&t);
	pthread_t handle[THREAD_NUM];
	thread_arg_t targ[THREAD_NUM];
	bool primes[DATA_NUM];
	int i;

	for(i=0; i<DATA_NUM; i++) {
		primes[i] = true;
	}

	for(i=0; i<THREAD_NUM; i++) {
		targ[i].id = i;
		targ[i].primes = primes;
		pthread_create(&handle[i], NULL, (void*)thread_func, (void*)&targ[i]);
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
	time = stop_timer(&t);
	print_timer(time);
	return 0;
}




