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

char* getStatus(bool acquired);
void printSmokingEvent(char* thread_name);
void printSemaphoreEvent(char* thread_name, char* sem_name, bool acquired);
void printTableClear(char* thread_name);