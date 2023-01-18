#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define AGENT "AGENT: "
#define SM_TOBACCO "SMOKER_WITH_TOBACCO: "
#define SM_PAPER "SMOKER_WITH_PAPER: "
#define SM_MATCHES "SMOKER_WITH_MATCHES: "
#define TOBACCO "tobacco."
#define PAPER "paper. "
#define MATCHES "matches."
#define ACQUIRED "acquired "
#define RELEASED "released "
#define CLEARED "table cleared."
#define SMOKING "smoking..."

char* getStatus(bool acquired);
void printSmokingEvent(char* thread_name);
void printSemaphoreEvent(char* thread_name, char* sem_name, bool acquired);
void printTableClear(char* thread_name);
