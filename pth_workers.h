#include <pthread.h>

typedef struct{
	pthread_t thread_id;
	void* routine;
} worker;