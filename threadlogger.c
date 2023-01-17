const char AGENT[] = "AGENT: ";
const char SM_TOBACCO[] = "SMOKER_WITH_TOBACCO: ";
const char SM_PAPER[] = "SMOKER_WITH_PAPER: ";
const char SM_MATCHES[] = "SMOKER_WITH_MATCHES: ";
const char TOBACCO[] = "tobacco.";
const char PAPER[] = "paper. ";
const char MATCHES[] = "matches.";
const char ACQUIRED[] = "acquired ";
const char RELEASED[] = "released ";
const char CLEARED[] = "cleared ";

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