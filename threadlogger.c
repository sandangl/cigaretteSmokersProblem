#include <threadlogger.h>

char* getStatus(bool acquired){
	if(acquired){
		return ACQUIRED;
	}
	return RELEASED;
}

void printSmokingEvent(char* thread_name){
	char msg[] = "";
	strcat(msg, strcat(thread_name, " smoking..."));
	printer(msg);
}

void printSemaphoreEvent(char* thread_name, char* sem_name, bool acquired){
	char msg[] = "";
	strcat(msg, strcat(thread_name, strcat(sem_name, getStatus(acquired))));
	printer(msg);
}

void printTableClear(char* thread_name){
	char msg[] = "";
	strcat(msg, strcat(thread_name, strcat(" table ", CLEARED)));
	printer(msg);
}

void printer(char* msg){
	for (size_t i = 0; i < strlen(msg); i++) {
       	printf("%c\n", i, msg[i]);
  	}
}