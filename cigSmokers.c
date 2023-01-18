#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "threadlogger.h"

sem_t tobacco;
sem_t paper;
sem_t matches;
sem_t placeOnTable;

void* agentRoutine(void* arg){
	int random = rand() % 3;
	if(random == 0){
		sem_post(&paper);
		sem_post(&matches);
		printSemaphoreEvent(AGENT, PAPER, false);
		printSemaphoreEvent(AGENT, MATCHES, false);
	}
	else if(random == 1){
		sem_post(&matches);
		sem_post(&tobacco);
		printSemaphoreEvent(AGENT, MATCHES, false);
		printSemaphoreEvent(AGENT, TOBACCO, false);
	}
	else{
		sem_post(&tobacco);
		sem_post(&paper);
		printSemaphoreEvent(AGENT, TOBACCO, false);
		printSemaphoreEvent(AGENT, PAPER, false);
	}
	sem_wait(&placeOnTable);
}

void* smokerTobaccoRoutine(void* arg){
	sem_wait(&paper);
	printSemaphoreEvent(SM_TOBACCO, PAPER, true);
	if(sem_trywait(&matches) == 0){
		printSemaphoreEvent(SM_TOBACCO, MATCHES, true);
		sem_post(&placeOnTable);
		printTableClear(SM_TOBACCO);
		printSmokingEvent(SM_TOBACCO);
		sleep(1);
	}
	else{
		sem_post(&paper);
		printSemaphoreEvent(SM_TOBACCO, PAPER, false);
	}
}

void* smokerPaperRoutine(void* arg){
	sem_wait(&tobacco);
	printSemaphoreEvent(SM_PAPER, TOBACCO, true);
	if(sem_trywait(&matches) == 0){
		printSemaphoreEvent(SM_PAPER, MATCHES, true);
		sem_post(&placeOnTable);
		printTableClear(SM_PAPER);
		printSmokingEvent(SM_PAPER);
		sleep(1);
	}
	else{
		sem_post(&tobacco);
		printSemaphoreEvent(SM_PAPER, TOBACCO, false);
	}
}

void* smokerMatchesRoutine(void* arg){
	sem_wait(&tobacco);
	printSemaphoreEvent(SM_MATCHES, TOBACCO, true);
	if(sem_trywait(&paper) == 0){
		printSemaphoreEvent(SM_MATCHES, PAPER, true);
		sem_post(&placeOnTable);
		printTableClear(SM_MATCHES);
		printSmokingEvent(SM_MATCHES);
		sleep(1);
	}
	else{
		sem_post(&tobacco);
		printSemaphoreEvent(SM_MATCHES, TOBACCO, false);
	}
}

int main(){
	pthread_t agent, smokerTobacco, smokerPaper, smokerMatches;
	
	//Semaphore initialization
	sem_init(&tobacco, 0, 0);
	sem_init(&paper, 0, 0);
	sem_init(&matches, 0, 0);
	sem_init(&placeOnTable, 0, 0);
	
	srand(time(NULL));
	
	//thread creation
	pthread_create(&agent, NULL, &agentRoutine, NULL);
	pthread_create(&smokerTobacco, NULL, &smokerTobaccoRoutine, NULL);
	pthread_create(&smokerPaper, NULL, &smokerPaperRoutine, NULL);
	pthread_create(&smokerMatches, NULL, &smokerMatchesRoutine, NULL);
	
	//thread termination
	pthread_join(agent, NULL);
	pthread_join(smokerTobacco, NULL);
	pthread_join(smokerPaper, NULL);
	pthread_join(smokerMatches, NULL);
	
	return 0;
}
