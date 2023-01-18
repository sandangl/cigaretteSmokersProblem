#include "threadlogger.h"

void printer(char* msg){
	printf("%s \n", msg);
}

char* getStatus(bool acquired){
	if(acquired){
		return ACQUIRED;
	}
	return RELEASED;
}

void printSmokingEvent(char* thread_name){
	char* msg = malloc(strlen(thread_name) + strlen(SMOKING));
	strcat(msg, thread_name);
	strcat(msg, SMOKING);
	printer(msg);
}

void printSemaphoreEvent(char* thread_name, char* sem_name, bool acquired){
	char* msg = malloc(strlen(thread_name) + strlen(sem_name) + strlen(getStatus(acquired));
	strcat(msg, thread_name);
	strcat(msg, getStatus(acquired));
	strcat(msg, sem_name);
	printer(msg);
}

void printTableClear(char* thread_name){
	char* msg = malloc(strlen(thread_name) + strlen(CLEARED));
	strcat(msg, thread_name);
	strcat(msg, CLEARED);
	printer(msg);
}
