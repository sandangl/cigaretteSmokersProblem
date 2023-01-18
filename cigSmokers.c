#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tobacco;
sem_t paper;
sem_t matches;
sem_t placeOnTable;

void* agentRoutine(void*arg){
	int random = rand() % 3;
	if(random == 0){
		printf("AGENT: put paper on table.\n");
		sem_post(&paper);
		printf("AGENT: put matches on table.\n");
		sem_post(&matches);
	}
	else if(random == 1){
		printf("AGENT: put matches on table.\n");
		sem_post(&matches);
		printf("AGENT: put tobacco on table.\n");
		sem_post(&tobacco);
	}
	else{
		printf("AGENT: put tobacco on table.\n");
		sem_post(&tobacco);
		printf("AGENT: put paper on table.\n");
		sem_post(&paper);
	}
	printf("AGENT: waiting for table to be cleared...\n");
	sem_wait(&placeOnTable);
}

void* smokerTobaccoRoutine(void* arg){
	sem_wait(&paper);
	printf("SMOKER_WITH_TOBACCO: took paper.\n");
	if(sem_trywait(&matches) == 0){
		printf("SMOKER_WITH_TOBACCO: took matches.\n");
		printf("SMOKER_WITH_TOBACCO: table cleared.\n");
		sem_post(&placeOnTable);
		printf("SMOKER_WITH_TOBACCO: smoking...\n");
		sleep(5);
	}
	else{
		printf("SMOKER_WITH_TOBACCO: put back paper.\n");
		sem_post(&paper);
	}
}

void* smokerPaperRoutine(void* arg){
	sem_wait(&tobacco);
	printf("SMOKER_WITH_PAPER: took tobacco.\n");
	if(sem_trywait(&matches) == 0){
		printf("SMOKER_WITH_PAPER: took matches.\n");
		printf("SMOKER_WITH_PAPER: table cleared.\n");
		sem_post(&placeOnTable);
		printf("SMOKER_WITH_PAPER: smoking...\n");
		sleep(5);
	}
	else{
		printf("SMOKER_WITH_PAPER: put back tobacco.\n");
		sem_post(&tobacco);
	}
}

void* smokerMatchesRoutine(void* arg){
	sem_wait(&tobacco);
	printf("SMOKER_WITH_MATCHES: took tobacco.\n");
	if(sem_trywait(&paper) == 0){
		printf("SMOKER_WITH_MATCHES: took paper.\n");
		printf("SMOKER_WITH_MATCHES: table cleared.\n");
		sem_post(&placeOnTable);
		printf("SMOKER_WITH_MATCHES: smoking...\n");
		sleep(5);
	}
	else{
		printf("SMOKER_WITH_MATCHES: put back tobacco.\n");
		sem_post(&tobacco);
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
