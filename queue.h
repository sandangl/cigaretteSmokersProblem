#include "pth_workers.h"
#define N 4

typedef struct {
	worker waiting[N];
	int ind;		
} queue;

void init(queue *q);
int enqueue(queue *q, worker thread);
worker dequeue(queue *q);