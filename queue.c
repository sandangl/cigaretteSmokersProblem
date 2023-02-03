#include "queue.h"

void init(queue *q){
	q->ind = 0;
}

int enqueue(queue *q, worker thread){
	if(q->ind == N || q->ind > N){
		return -1;
	}
	q->waiting[q->ind] = thread;
	q->ind = q->ind+1;
	return 0;
}

worker dequeue(queue *q){
	worker deq = q->waiting[0];
	int i;
	for(i = 0; i<q->ind-1; i++){
		q->waiting[i] = q->waiting[i+1];
	}
	q->ind = q->ind-1;
	return deq;
}