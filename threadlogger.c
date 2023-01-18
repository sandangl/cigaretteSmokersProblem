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
	char msg[255];
	strcat(msg, thread_name);
	strcat(msg, "smoking...");
	printer(msg);
}

void printSemaphoreEvent(char* thread_name, char* sem_name, bool acquired){
	char msg[255];
	strcat(msg, thread_name);
	strcat(msg, getStatus(acquired));
	strcat(msg, sem_name);
	printer(msg);
}

void printTableClear(char* thread_name){
	char msg[255];
	strcat(msg, thread_name);
	strcat(msg, CLEARED);
	printer(msg);
}
